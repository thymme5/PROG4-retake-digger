#pragma once

#include <string>
#include <unordered_map>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

struct Mix_Chunk;

namespace dae
{
    class SoundService
    {
    public:
        virtual ~SoundService() = default;
        virtual void PlaySound(const std::string& soundFile) = 0;
        virtual void LoadSound(const std::string& soundFile) = 0;
        virtual void StopAllSounds() = 0;

        virtual void SetMuted(bool muted) = 0;
        virtual bool IsMuted() const = 0;
        virtual bool IsPlaying(const std::string& soundFile) = 0;
        virtual void SetVolume(int volume) = 0; // volume: 0 (mute) to MIX_MAX_VOLUME (128)

    };

    class SDLMixerSoundService final : public SoundService
    {
    public:
        SDLMixerSoundService();
        ~SDLMixerSoundService() override;

        void PlaySound(const std::string& soundFile) override;
        void LoadSound(const std::string& soundFile) override;
        void StopAllSounds() override;

        void SetMuted(bool muted) override;
        bool IsMuted() const override;
        bool IsPlaying(const std::string& soundFile) override;
        void SetVolume(int volume) override;

    private:
        void ProcessQueue();

        enum class SoundCommandType { Play, Load };

        struct SoundCommand
        {
            SoundCommandType type;
            std::string soundFile;
        };

        std::queue<SoundCommand> m_CommandQueue;
        std::mutex m_Mutex;
        std::condition_variable m_CondVar;
        std::atomic<bool> m_Running{ true };
        std::thread m_WorkerThread;
        std::unordered_map<std::string, int> m_SoundToChannel;

        std::atomic<bool> m_IsMuted{ false };

        std::unordered_map<std::string, Mix_Chunk*> m_LoadedSounds;
        int m_Volume = 128;

    };

    class ServiceLocator
    {
    public:
        static SoundService* GetSoundService();
        //static void RegisterSoundService(SoundService* service);
        static void RegisterSoundService(std::unique_ptr<SoundService> service);
        static void Destroy();

    private:
        static std::unique_ptr<SoundService> m_pSoundService;
    };
}
