#include "LevelManager.h"
#include "TileManager.h"
#include "GameModeManager.h"
#include "SinglePlayerMode.h"
#include "CoopMode.h"
#include "VersusMode.h"
#include <iostream>

void LevelManager::Initialize()
{
    m_CurrentLevelIndex = 1;
    LoadLevel(m_CurrentLevelIndex);
}

void LevelManager::LoadLevel(int index)
{
    std::cout << "[LevelManager] Loading level " << index << "\n";
    ResetLevelState();

    auto* currentMode = GameModeManager::GetInstance().GetCurrentGameMode();
    if (dynamic_cast<SinglePlayerMode*>(currentMode))
    {
        GameModeManager::GetInstance().SetMode(std::make_unique<SinglePlayerMode>(m_CurrentLevelIndex));
    }
    else if (dynamic_cast<CoopMode*>(currentMode))
    {
        GameModeManager::GetInstance().SetMode(std::make_unique<CoopMode>(m_CurrentLevelIndex));
    }
    else if (dynamic_cast<VersusMode*>(currentMode))
    {
        GameModeManager::GetInstance().SetMode(std::make_unique<VersusMode>(m_CurrentLevelIndex));
    }
}

void LevelManager::LoadNextLevel()
{
    if (m_CurrentLevelIndex + 1 < m_TotalLevels)
    {
        ++m_CurrentLevelIndex;
        LoadLevel(m_CurrentLevelIndex);
    }
    else
    {
        std::cout << "[LevelManager] All levels completed!\n";
    }
}

void LevelManager::OnNotify(dae::Event event, dae::GameObject* source)
{
    if (event == dae::Event::LevelCompleted)
    {
        HandleLevelCompleted(source);
    }
}

void LevelManager::HandleLevelCompleted(dae::GameObject* player)
{
    std::cout << "[LevelManager] Level completed by player!\n";
    LoadNextLevel();
}

void LevelManager::ResetLevelState()
{

}
