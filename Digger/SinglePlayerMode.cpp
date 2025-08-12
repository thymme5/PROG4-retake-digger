#include "SinglePlayerMode.h"
#include "DiggerSceneBuilder.h"
#include "InputManager.h"
#include "SceneManager.h"
#include <sstream>
#include <filesystem>
#include <iostream>

SinglePlayerMode::SinglePlayerMode(int levelIndex)
    : m_LevelIndex(levelIndex)
{
}

void SinglePlayerMode::Enter()
{
    auto& rm = dae::ResourceManager::GetInstance();

    std::stringstream ss;
    ss << "Level0" << m_LevelIndex << "Solo.json";
    std::filesystem::path levelPath = rm.GetDataPath() / "levels" / ss.str();

    if (!std::filesystem::exists(levelPath)) {
        std::cerr << "Failed to open level file: " << levelPath.string() << "\n";
        return;
    }

    DiggerSceneBuilder::CreateSinglePlayerScene(
        dae::SceneManager::GetInstance().CreateScene(m_SceneName),
        levelPath.string()
    );
}

void SinglePlayerMode::Exit()
{
    dae::InputManager::GetInstance().ClearAllBindings();
    dae::SceneManager::GetInstance().MarkSceneForDeletion(m_SceneName);
}

void SinglePlayerMode::Update()
{
}
