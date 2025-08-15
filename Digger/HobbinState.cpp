#include "HobbinState.h"
#include "EnemyComponent.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "Timer.h"
#include "TextureComponent.h"
#include "PlayerComponent.h"

void HobbinState::Enter(EnemyComponent& enemy)
{
    // Move as soon as we enter this state
    m_MoveCooldown = 0.0f;

    if (enemy.GetOwner()->HasComponent<dae::TextureComponent>())
    {
		enemy.GetOwner()->GetComponent<dae::TextureComponent>()->SetTexture("hobbin.png");
    }
}

void HobbinState::Update(EnemyComponent& enemy)
{
    const float dt = Timer::GetDeltaTime();

    m_MoveCooldown -= dt;
    if (m_MoveCooldown > 0.0f) return;
    m_MoveCooldown = m_MoveInterval;

    auto [dr, dc] = enemy.BestStepTowardTarget(false);
    if (dr == 0 && dc == 0) return;

    const int nr = enemy.GetRow() + dr;
    const int nc = enemy.GetCol() + dc;

    auto tile = TileManager::GetInstance().GetTile(nr, nc);
    if (!tile) return;

    // Hobbin digs like a good boy
    if (!tile->IsDug())
        tile->SetDug(true);

    enemy.MoveBy(dr, dc);
}

