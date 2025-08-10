#pragma once
#include "EnemyState.h"
#include <memory>

class NobbinState final : public EnemyState
{
public:
    NobbinState(int framesToHobbin = 300) // e.g., 5 seconds at 60fps
        : m_FramesToHobbin(framesToHobbin) {
    }

    void Enter(EnemyComponent& enemy) override;
    void Update(EnemyComponent& enemy) override;

private:
    int m_FramesRemaining{};
    int m_FramesToHobbin;
};
