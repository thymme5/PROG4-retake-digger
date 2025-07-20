#pragma once
#include "GameModeManager.h"
#include "InputManager.h"

void GameModeManager::SetMode(std::unique_ptr<GameMode> newMode)
{
    if (m_CurrentMode)
        m_CurrentMode->Exit();

    m_CurrentMode = std::move(newMode);

    if (m_CurrentMode)
        m_CurrentMode->Enter();
}

void GameModeManager::Update()
{
    if (m_CurrentMode)
        m_CurrentMode->Update();
}

GameMode* GameModeManager::GetCurrentGameMode() const noexcept
{
    return m_CurrentMode.get();
}
