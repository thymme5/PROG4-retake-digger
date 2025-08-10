#include "HobbinState.h"
#include "NobbinState.h"
#include "EnemyComponent.h"
#include "TileManager.h"
#include "TileComponent.h"

void HobbinState::Enter(EnemyComponent& enemy)
{ 
	std::cout << "HobbinState: Entering state\n";
    m_FramesRemaining = m_FramesToNobbin;
}

void HobbinState::Update(EnemyComponent& enemy)
{
    // Countdown to revert
    if (--m_FramesRemaining <= 0)
    {
        enemy.SetState(std::make_unique<NobbinState>());
        return;
    }

    // Move through dug + undug tiles
    auto [dr, dc] = enemy.BestStepTowardTarget(false);
    if (dr != 0 || dc != 0)
    {
        int nr = enemy.GetRow() + dr;
        int nc = enemy.GetCol() + dc;

        // Dig if needed
        if (auto tile = TileManager::GetInstance().GetTile(nr, nc))
        {
            if (!tile->IsDug())
                tile->SetDug(true);
        }

        enemy.MoveBy(dr, dc);
    }
}
