#include "SoundService.h"
#include <SDL_mixer.h>
#include <iostream>
#include <algorithm>

namespace dae
{
    std::unique_ptr<SoundService> ServiceLocator::m_pSoundService = nullptr;

    void ServiceLocator::RegisterSoundService(std::unique_ptr<SoundService> service)
    {
        m_pSoundService = std::move(service); // transfers ownership
    }

    SoundService* ServiceLocator::GetSoundService()
    {
        return m_pSoundService.get();
    }
    void ServiceLocator::Destroy()
    {
        m_pSoundService.reset();
    }
    SDLMixerSoundService::SDLMixerSoundService()
    {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        }
        m_WorkerThread = std::thread(&SDLMixerSoundService::ProcessQueue, this);
    }

    SDLMixerSoundService::~SDLMixerSoundService()
    {
        m_Running = false;
        m_CondVar.notify_all();
        if (m_WorkerThread.joinable())
            m_WorkerThread.join();

        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            while (!m_CommandQueue.empty())
                m_CommandQueue.pop();
        }

        for (auto& pair : m_LoadedSounds)
            Mix_FreeChunk(pair.second);
        m_LoadedSounds.clear();

        m_SoundToChannel.clear();

        Mix_CloseAudio();
    }

    void SDLMixerSoundService::SetMuted(bool muted) 
    { 
        std::cout << "muted: " << std::boolalpha << muted << std::endl;

        m_IsMuted = muted;
    }
    bool SDLMixerSoundService::IsMuted() const 
    { 
        return m_IsMuted;
    }
    void SDLMixerSoundService::PlaySound(const std::string& soundFile)
    {
        if (m_IsMuted) return;

        std::lock_guard<std::mutex> lock(m_Mutex);
        m_CommandQueue.push({ SoundCommandType::Play, soundFile });
        m_CondVar.notify_one();
    }

    void SDLMixerSoundService::LoadSound(const std::string& soundFile)
    {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_CommandQueue.push({ SoundCommandType::Load, soundFile });
        }
        m_CondVar.notify_one();
    }
    bool SDLMixerSoundService::IsPlaying(const std::string& soundFile)
    {
        const auto it = m_SoundToChannel.find(soundFile);
        if (it == m_SoundToChannel.end()) return false;

        int channel = it->second;
        return Mix_Playing(channel) != 0;
    }
    void SDLMixerSoundService::StopAllSounds()
    {
        Mix_HaltChannel(-1);
    }

    void SDLMixerSoundService::SetVolume(int volume)
    {
        m_Volume = std::clamp(volume, 0, MIX_MAX_VOLUME);

        for (auto& [_, chunk] : m_LoadedSounds)
        {
            Mix_VolumeChunk(chunk, m_Volume);
        }

        std::cout << "Volume set to: " << m_Volume << '\n';
    }

    void SDLMixerSoundService::ProcessQueue()
    {
        while (m_Running)
        {
            std::unique_lock<std::mutex> lock(m_Mutex);
            m_CondVar.wait(lock, [this] { return !m_CommandQueue.empty() || !m_Running; });

            while (!m_CommandQueue.empty())
            {
                auto cmd = m_CommandQueue.front();
                m_CommandQueue.pop();
                lock.unlock();

                if (cmd.type == SoundCommandType::Load)
                {
                    if (m_LoadedSounds.find(cmd.soundFile) == m_LoadedSounds.end())
                    {
                        Mix_Chunk* chunk = Mix_LoadWAV(cmd.soundFile.c_str());
                        if (chunk)
                        {
                            Mix_VolumeChunk(chunk, m_Volume); 
                            m_LoadedSounds[cmd.soundFile] = chunk;
                        }
                        else
                            std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
                    }
                }
                else if (cmd.type == SoundCommandType::Play)
                {
                    auto it = m_LoadedSounds.find(cmd.soundFile);
                    if (it != m_LoadedSounds.end())
                        Mix_PlayChannel(-1, it->second, 0);
                    else
                        std::cerr << "Sound not loaded: " << cmd.soundFile << std::endl;
                    
                    int channel = Mix_PlayChannel(-1, it->second, 0);
                    if (channel != -1)
                    {
                        m_SoundToChannel[cmd.soundFile] = channel;
                    }

                }

                lock.lock();
            }
        }
    }
}