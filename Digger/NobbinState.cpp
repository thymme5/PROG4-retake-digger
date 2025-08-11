#include "NobbinState.h"
#include "HobbinState.h"
#include "EnemyComponent.h"
#include "Timer.h"

class HobbinState;

void NobbinState::Enter(EnemyComponent& /*enemy*/)
{
    m_SecondsRemaining = m_SecondsToHobbin;
    m_MoveCooldown = 0.0f; // move asap after entering
}

void NobbinState::Update(EnemyComponent& enemy)
{
    const float dt = Timer::GetDeltaTime();

    // evolve countdown
    m_SecondsRemaining -= dt;
    if (m_SecondsRemaining <= 0.0f)
    {
        enemy.SetState(std::make_unique<HobbinState>());
        return;
    }

    // move throttle
    m_MoveCooldown -= dt;
    if (m_MoveCooldown > 0.0f) return;
    m_MoveCooldown = m_MoveInterval;

    // tunnels-only step; if no valid step, stay put
    auto [dr, dc] = enemy.BestStepTowardTarget(true);
    if (dr || dc) enemy.MoveBy(dr, dc);
}
