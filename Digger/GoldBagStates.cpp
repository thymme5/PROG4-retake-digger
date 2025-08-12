#include "GoldBagStates.h"
#include "GoldBagComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "TileManager.h"


//
// IdleState
//
void IdleState::Enter(GoldBagComponent& goldBag)
{

}

void IdleState::Update(GoldBagComponent& goldBag)
{
    const int belowRow = goldBag.GetRow() + 1;
    const int col = goldBag.GetCol();

    auto belowTile = TileManager::GetInstance().GetTile(belowRow, col);
    if (!belowTile || belowTile->IsDug())
    {
        goldBag.SetState(std::make_unique<FallingState>());
    }
}

void IdleState::Exit(GoldBagComponent& goldBag)
{

}

//
// FallingState
//
void FallingState::Enter(GoldBagComponent& goldBag)
{
    goldBag.Fall();
}

void FallingState::Update(GoldBagComponent& goldBag)
{
    if (goldBag.IsMidFall()) return;

    const int currRow = goldBag.GetRow();
    const int col = goldBag.GetCol();
    const int belowRow = currRow + 1;

    if (!TileManager::GetInstance().IsValidTile(belowRow, col))
    {
        // Out of bounds, break
        goldBag.SetState(std::make_unique<BrokenState>());
        return;
    }

    auto belowTile = TileManager::GetInstance().GetTile(belowRow, col);
    bool canFall = (!belowTile || belowTile->IsDug());

    if (canFall)
    {
        goldBag.Fall();
    }
    else
    {
        // Landed on solid tile — now check fall distance
        if (goldBag.GetFallDistance() >= 2)
        {
            goldBag.SetState(std::make_unique<BrokenState>());
        }
        else
        {
            goldBag.ResetFallDistance();
            goldBag.SetState(std::make_unique<IdleState>());
        }
    }
}


void FallingState::Exit(GoldBagComponent& goldBag)
{

}

//
// BrokenState
//
void BrokenState::Enter(GoldBagComponent& goldBag)
{
	goldBag.GetOwner()->GetComponent<dae::TextureComponent>()->SetTexture("gold.png");
}

void BrokenState::Update(GoldBagComponent& goldBag)
{
    // Maybe animate or just stay static
}

void BrokenState::Exit(GoldBagComponent& goldBag)
{
    // Likely never called unless bag respawns
}
