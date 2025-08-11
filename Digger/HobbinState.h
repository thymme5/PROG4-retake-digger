#pragma once
#include "EnemyState.h"
#include <memory>

class HobbinState final : public EnemyState
{
public:
    HobbinState() = default;

    void Enter(EnemyComponent& enemy) override;
    void Update(EnemyComponent& enemy) override;
    void Exit(EnemyComponent& enemy) override {}

private:
    float m_MoveInterval{ 0.50f };
    float m_MoveCooldown{ 0.0f };
};
