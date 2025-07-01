#include "QbertSoundLibrary.h"
#include "ResourceManager.h"

std::unordered_map<SoundID, std::string> QbertSoundLibrary::m_SoundMap{};

void QbertSoundLibrary::LoadAllSounds()
{
    const auto& basePath = dae::ResourceManager::GetInstance().GetDataPath();

    Register(SoundID::QbertJump, (basePath / "Sounds/QBert Jump.wav").string());
    Register(SoundID::CoilyEggJump, (basePath / "Sounds/Coily Egg Jump.wav").string());
    Register(SoundID::CoilySnakeJump, (basePath / "Sounds/Coily Snake Jump.wav").string());
    Register(SoundID::RoundComplete, (basePath / "Sounds/Round Complete Tune.wav").string());
    Register(SoundID::ChangeSelection, (basePath / "Sounds/Change Selection.wav").string());
    Register(SoundID::ClearDisks, (basePath / "Sounds/Clear Disks.wav").string());
    Register(SoundID::CoilyFall, (basePath / "Sounds/Coily Fall.wav").string());
    Register(SoundID::DemoSound, (basePath / "Sounds/Demo Sound.wav").string());
    Register(SoundID::DiskLand, (basePath / "Sounds/Disk Land.wav").string());
    Register(SoundID::DiskLift, (basePath / "Sounds/Disk Lift.wav").string());
    Register(SoundID::LevelScreenTune, (basePath / "Sounds/Level Screen Tune.wav").string());
    Register(SoundID::OtherFoesJump, (basePath / "Sounds/Other Foes Jump.wav").string());
    Register(SoundID::QbertFall, (basePath / "Sounds/QBert Fall.wav").string());
    Register(SoundID::QbertHit, (basePath / "Sounds/QBert Hit.wav").string());
    Register(SoundID::SlickSamCaught, (basePath / "Sounds/SlickSam Caught.wav").string());
    Register(SoundID::Swearing, (basePath / "Sounds/Swearing.wav").string());
}

void QbertSoundLibrary::Play(SoundID id)
{
    auto* service = dae::ServiceLocator::GetSoundService();
    if (!service) return;

    const auto& it = m_SoundMap.find(id);
    if (it != m_SoundMap.end())
    {
        service->PlaySound(it->second);
    }
}

void QbertSoundLibrary::Register(SoundID id, const std::string& path)
{
    auto* service = dae::ServiceLocator::GetSoundService();
    if (!service) return;
    m_SoundMap[id] = path;
    service->LoadSound(path);
}

bool QbertSoundLibrary::IsPlaying(SoundID id)
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
