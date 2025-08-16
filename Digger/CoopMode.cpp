#include "CoopMode.h"
#include "DiggerSceneBuilder.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "DiggerSoundLibrary.h"

#include <filesystem>
#include <sstream>
#include <iostream>

CoopMode::CoopMode(int levelIndex)
    : m_LevelIndex(levelIndex)
{
}

void CoopMode::Enter()
{
    auto& rm = dae::ResourceManager::GetInstance();
    DiggerSoundLibrary::Play(SoundID::BackgroundMusic);

    std::stringstream ss;
    ss << "Level0" << m_LevelIndex << "Coop.json";
    std::filesystem::path levelPath = rm.GetDataPath() / "levels" / ss.str();

    if (!std::filesystem::exists(levelPath)) {
        std::cerr << "Failed to open level file: " << levelPath.string() << "\n";
        return;
    }

    DiggerSceneBuilder::CreateCoopScene(
        dae::SceneManager::GetInstance().CreateScene(m_SceneName),
        levelPath.string()
    );
}

void CoopMode::Exit()
{
    dae::InputManager::GetInstance().ClearAllBindings();
    dae::SceneManager::GetInstance().MarkSceneForDeletion(m_SceneName);
}

void CoopMode::Update()
{
}
