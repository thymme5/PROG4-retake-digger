#include "GoldBagStates.h"
#include "GoldBagComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "TileManager.h"

constexpr int TILE_SIZE = 48;

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
    const int currRow = goldBag.GetRow();
    const int col = goldBag.GetCol();
    const int belowRow = currRow + 1;

    auto belowTile = TileManager::GetInstance().GetTile(belowRow, col);

    // Check if gold bag should continue falling
    bool canFall = (!belowTile || belowTile->IsDug());

    if (canFall)
    {
        goldBag.Fall(); // Move down, increase fall distance
        goldBag.GetOwner()->SetLocalPosition(col * TILE_SIZE, (currRow + 1) * TILE_SIZE);
    }
    else
    {
        // Stop falling, evaluate how far it fell
        if (goldBag.GetFallDistance() >= 2)
        {
            goldBag.SetState(std::make_unique<BrokenState>());
        }
        else
        {
            goldBag.ResetFallDistance(); // Didn’t fall far enough to break
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
