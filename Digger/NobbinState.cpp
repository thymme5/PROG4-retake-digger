#include "NobbinState.h"
#include "HobbinState.h"
#include "EnemyComponent.h"

void NobbinState::Enter(EnemyComponent& enemy)
{
    m_FramesRemaining = m_FramesToHobbin;
}

void NobbinState::Update(EnemyComponent& enemy)
{
    // Countdown to evolve
    if (--m_FramesRemaining <= 0)
    {
        enemy.SetState(std::make_unique<HobbinState>());
        return;
    }

    // Move only through dug tunnels
    auto [dr, dc] = enemy.BestStepTowardTarget(true);
    if (dr != 0 || dc != 0)
        enemy.MoveBy(dr, dc);
}
