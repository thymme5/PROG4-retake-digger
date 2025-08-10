#include "NobbinState.h"
#include "HobbinState.h"
#include "EnemyComponent.h"
#include "TileManager.h"
#include "Timer.h" 

void NobbinState::Enter(EnemyComponent& enemy)
{
    m_SecondsRemaining = m_SecondsToHobbin;
    m_MoveCooldown = 0.f;
}

void NobbinState::Update(EnemyComponent& enemy)
{
    float dt = Timer::GetDeltaTime();

    // Countdown to evolve into Hobbin
    m_SecondsRemaining -= dt;
    if (m_SecondsRemaining <= 0.f)
    {
        enemy.SetState(std::make_unique<HobbinState>());
        return;
    }

    // Handle movement cooldown
    m_MoveCooldown -= dt;
    if (m_MoveCooldown > 0.f)
        return;

    m_MoveCooldown = m_MoveInterval;

    // Nobbin: Move only through dug tunnels
    auto [dr, dc] = enemy.BestStepTowardTarget(true); // true = tunnelsOnly
    if (dr != 0 || dc != 0)
        enemy.MoveBy(dr, dc);
}
