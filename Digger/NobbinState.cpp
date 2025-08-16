#include "NobbinState.h"
#include "HobbinState.h"
#include "EnemyComponent.h"
#include "Timer.h"
#include "TileManager.h"
#include "PlayerComponent.h"

// FIRST STATE
class HobbinState;

void NobbinState::Enter(EnemyComponent& /*enemy*/)
{
    m_SecondsRemaining = m_SecondsToHobbin;
    m_MoveCooldown = 0.0f; // move asap after entering
}

void NobbinState::Update(EnemyComponent& enemy)
{
    const float dt = Timer::GetDeltaTime();

    m_SecondsRemaining -= dt;
    if (m_SecondsRemaining <= 0.0f)
    {
        enemy.SetState(std::make_unique<HobbinState>());
        return;
    }

    m_MoveCooldown -= dt;
    if (m_MoveCooldown > 0.0f) return;
    m_MoveCooldown = m_MoveInterval;

    if (enemy.IsMoving()) return;

    auto [dr, dc] = enemy.BestStepTowardTarget(true);
    if (dr == 0 && dc == 0) return;

    const int nr = enemy.GetRow() + dr;
    const int nc = enemy.GetCol() + dc;

    auto tile = TileManager::GetInstance().GetTile(nr, nc);
    if (!tile || !tile->IsDug()) return;

    enemy.MoveBy(dr, dc);
}

