#include "DiggerSoundLibrary.h"
#include "ResourceManager.h"

std::unordered_map<SoundID, std::string> DiggerSoundLibrary::m_SoundMap{};

void DiggerSoundLibrary::LoadAllSounds()
{
    const auto& basePath = dae::ResourceManager::GetInstance().GetDataPath();

    //Register(SoundID::QbertJump, (basePath / "Sounds/QBert Jump.wav").string());
}

void DiggerSoundLibrary::Play(SoundID id)
{
    auto* service = dae::ServiceLocator::GetSoundService();
    if (!service) return;

    const auto& it = m_SoundMap.find(id);
    if (it != m_SoundMap.end())
    {
        service->PlaySound(it->second);
    }
}

void DiggerSoundLibrary::Register(SoundID id, const std::string& path)
{
    auto* service = dae::ServiceLocator::GetSoundService();
    if (!service) return;
    m_SoundMap[id] = path;
    service->LoadSound(path);
}

bool DiggerSoundLibrary::IsPlaying(SoundID id)
{
    auto* service = dae::ServiceLocator::GetSoundService();
    if (!service) return false;

    const auto it = m_SoundMap.find(id);
    if (it != m_SoundMap.end())
    {
        return service->IsPlaying(it->second);
    }

    return false;
}
