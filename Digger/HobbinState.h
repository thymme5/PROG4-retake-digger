#pragma once
#include "EnemyState.h"
#include <memory>

class HobbinState final : public EnemyState
{
public:
    HobbinState(int framesToNobbin = 300) // e.g., 5 seconds at 60fps
        : m_FramesToNobbin(framesToNobbin) {
    }

    void Enter(EnemyComponent& enemy) override;
    void Update(EnemyComponent& enemy) override;

private:
    int m_FramesRemaining{};
    int m_FramesToNobbin;
};
