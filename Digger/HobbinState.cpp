#include "HobbinState.h"
#include "EnemyComponent.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "Timer.h"

void HobbinState::Enter(EnemyComponent& /*enemy*/)
{
    // Move as soon as we enter this state
    m_MoveCooldown = 0.0f;
}

void HobbinState::Update(EnemyComponent& enemy)
{
    const float dt = Timer::GetDeltaTime();

    // throttle movement
    m_MoveCooldown -= dt;
    if (m_MoveCooldown > 0.0f) return;
    m_MoveCooldown = m_MoveInterval;

    // Hobbin can move anywhere: dig destination if needed, then move
    auto [dr, dc] = enemy.BestStepTowardTarget(/*tunnelsOnly=*/false);
    if (dr || dc)
    {
        const int nr = enemy.GetRow() + dr;
        const int nc = enemy.GetCol() + dc;

        if (auto tile = TileManager::GetInstance().GetTile(nr, nc))
        {
            if (!tile->IsDug())
                tile->SetDug(true);
        }

        enemy.MoveBy(dr, dc);
    }
}
