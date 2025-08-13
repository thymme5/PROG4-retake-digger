#pragma once
#include "PlayerState.h"

class DeadState final : public PlayerState
{
public:
    void Enter(PlayerComponent& player) override;
    void Update(PlayerComponent& player) override;
    void Exit(PlayerComponent& player) override;
private:
    float m_DeathTimer = 0.f;
    float m_PulseTimer = 0.f;
    bool m_IsVisible = true;
    const float m_RespawnDelay = 5.f;
    const float m_PulseInterval = 0.3f;
};

