#pragma once
#include "EnemyState.h"
#include <memory>

// First stage state: Nobbin (cannot dig)
class NobbinState final : public EnemyState
{
public:
    explicit NobbinState(float secondsToHobbin = 15.0f)
        : m_SecondsToHobbin(secondsToHobbin),
        m_SecondsRemaining(secondsToHobbin)
    {
    }

    void Enter(EnemyComponent& enemy) override;
    void Update(EnemyComponent& enemy) override;

private:
    float m_SecondsRemaining;
    float m_SecondsToHobbin;    

    //TODO: tweak values
    float m_MoveInterval{ 0.5f };
    float m_MoveCooldown{ 0.f };
};
