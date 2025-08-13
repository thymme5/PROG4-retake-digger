#include "LevelManager.h"
#include "TileManager.h"
#include "GameModeManager.h"
#include "SinglePlayerMode.h"
#include "CoopMode.h"
#include "VersusMode.h"

#include <iostream>

static bool s_IsAlive = false;

LevelManager::LevelManager() { s_IsAlive = true; }
LevelManager::~LevelManager() { s_IsAlive = false; }

bool LevelManager::IsAlive() { return s_IsAlive; }


void LevelManager::Initialize()
{
    m_CurrentLevelIndex = 1;
    LoadLevel(m_CurrentLevelIndex);
}

void LevelManager::LoadLevel(int index)
{
    std::cout << "[LevelManager] Loading level " << index << "\n";
	Notify(dae::Event::LevelCompleted, nullptr);
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
    else // Game done
	{		
        Notify(dae::Event::GameCompleted, nullptr);
		m_CurrentLevelIndex = 1; // Reset for next game
        GameModeManager::GetInstance().SetMode(std::make_unique<MainMenu>());
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
