#include "VersusMode.h"
#include "DiggerSceneBuilder.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "DiggerSoundLibrary.h"
#include <sstream>
#include <filesystem>
#include <iostream>

VersusMode::VersusMode(int levelIndex)
    : m_LevelIndex(levelIndex)
{
}

void VersusMode::Enter()
{
    auto& rm = dae::ResourceManager::GetInstance();
    DiggerSoundLibrary::Play(SoundID::BackgroundMusic);

    std::stringstream ss;
    ss << "Level0" << m_LevelIndex << "Versus.json";
    std::filesystem::path levelPath = rm.GetDataPath() / "levels" / ss.str();

    if (!std::filesystem::exists(levelPath)) {
        std::cerr << "[VersusMode] Failed to open level file: " << levelPath.string() << "\n";
        return;
    }

    std::cout << "[VersusMode] Loading level " << m_LevelIndex << "\n";

    DiggerSceneBuilder::CreateVersusScene(
        dae::SceneManager::GetInstance().CreateScene(m_SceneName),
        levelPath.string()
    );
}

void VersusMode::Exit()
{
    dae::InputManager::GetInstance().ClearAllBindings();
    dae::SceneManager::GetInstance().MarkSceneForDeletion(m_SceneName);
}

void VersusMode::Update()
{
}
