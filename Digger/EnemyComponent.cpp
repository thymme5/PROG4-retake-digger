#include "EnemyComponent.h"
#include "EnemyState.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "GameObject.h"
#include "NobbinState.h" 

EnemyComponent::EnemyComponent(dae::GameObject& owner, int startRow, int startCol)
    : Component(owner), m_Row(startRow), m_Col(startCol)
{
    // first state
    SetState(std::make_unique<NobbinState>());
}

void EnemyComponent::Update()
{ 
    if (m_pCurrentState)
        m_pCurrentState->Update(*this);
}

void EnemyComponent::SetTile(int row, int col)
{
    m_Row = row;
    m_Col = col;

    constexpr int TILE_SIZE = 48;
    GetOwner()->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);
}

void EnemyComponent::MoveBy(int dr, int dc)
{
    m_LastDr = dr;
    m_LastDc = dc;
    SetTile(m_Row + dr, m_Col + dc);
}

void EnemyComponent::SetState(std::unique_ptr<EnemyState> newState)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);

    m_pCurrentState = std::move(newState);

    if (m_pCurrentState)
        m_pCurrentState->Enter(*this);
}

std::pair<int, int> EnemyComponent::BestStepTowardTarget(bool tunnelsOnly) const
{
    // down, up, right, left
    static const int dirs[4][2] = { {1,0}, {-1,0}, {0,1}, {0,-1} };
    int bestDr = 0, bestDc = 0;
    int bestDist = 1'000'000;

    for (auto& d : dirs)
    {
        int dr = d[0], dc = d[1];

        if (dr == -m_LastDr && dc == -m_LastDc)
            continue;

        int nr = m_Row + dr;
        int nc = m_Col + dc;

        auto tile = TileManager::GetInstance().GetTile(nr, nc);
        if (!tile)
            continue;

        if (tunnelsOnly && !tile->IsDug())
            continue;

        int dist = std::abs(m_TargetRow - nr) + std::abs(m_TargetCol - nc);
        if (dist < bestDist)
        {
            bestDist = dist;
            bestDr = dr;
            bestDc = dc;
        }
    }

    return { bestDr, bestDc };
}

bool EnemyComponent::ShouldStepThisFrame(int framesPerStep)
{
    if (++m_FrameCounter >= framesPerStep)
    {
        m_FrameCounter = 0;
        return true;
    }
    return false;
}
