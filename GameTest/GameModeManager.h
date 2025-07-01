#pragma once
#include <memory>
#include "GameMode.h"
#include "Singleton.h"

class GameMode;

class GameModeManager final : public dae::Singleton<GameModeManager>
{
public:
    void SetMode(std::unique_ptr<GameMode> newMode);
    GameMode* GetCurrentGameMode() const noexcept;
    void Update();

private:
    friend class dae::Singleton<GameModeManager>;
    GameModeManager() = default;

    std::unique_ptr<GameMode> m_CurrentMode;
};
