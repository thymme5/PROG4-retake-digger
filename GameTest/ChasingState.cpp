#include "ChasingState.h"
#include "CoilyComponent.h"
#include "TileComponent.h"
#include "QbertMoveComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "Observer.h"

void ChasingState::Enter(CoilyComponent&)
{
}

void ChasingState::Update(CoilyComponent& coily)
{
    if (coily.GetPaused() || coily.IsJumping()) return;

    if (coily.IsPlayerControlled()) return;

    auto coilyTile = coily.GetCoilyTile();
    auto qbertTile = coily.GetQbertTile();

    if (!coilyTile || !qbertTile) return;

    auto [cr, cc] = coilyTile->GetGridPosition(); //coily row, coily col
    auto [qr, qc] = qbertTile->GetGridPosition(); // qbert row, qbert col

    // already on the same tile
    if (qr == cr && qc == cc)
    {
        return; 
    }

    std::cout << "Coily at (" << cr << "," << cc << ") -> Q*bert at (" << qr << "," << qc << ")\n";

    int rowDiff = qr - cr;
    int colDiff = qc - cc;

    Direction dir;

    if (rowDiff > 0 && colDiff > 0)
        dir = Direction::DownRight;
    else if (rowDiff > 0 && colDiff <= 0)
        dir = Direction::DownLeft;
    else if (rowDiff <= 0 && colDiff > 0)
        dir = Direction::UpRight;
    else // rowDiff <= 0 && colDiff <= 0
        dir = Direction::UpLeft;

    coily.TryMove(dir);
}

void ChasingState::Exit(CoilyComponent&)
{
}
