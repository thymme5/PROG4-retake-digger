#include "EnemyComponent.h"
#include "EnemyState.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "GameObject.h"
#include "NobbinState.h" 
#include "PlayerComponent.h"

// --- tiny registry of all enemies ---
static std::vector<EnemyComponent*>& EnemyList() {
    static std::vector<EnemyComponent*> s;
    return s;
}

EnemyComponent::EnemyComponent(dae::GameObject& owner, int startRow, int startCol)
    : Component(owner), m_Row(startRow), m_Col(startCol)
{
    // first state is always nobbin
    // (this kept confusing me)
    EnemyList().push_back(this);
    SetState(std::make_unique<NobbinState>());
    TileManager::GetInstance().RegisterEnemy(m_Row, m_Col, GetOwner());
}

EnemyComponent::~EnemyComponent()
{
    auto& v = EnemyList();
    v.clear(); 
    TileManager::GetInstance().RemoveEnemy(m_Row, m_Col, GetOwner());
}
void EnemyComponent::Update()
{ 
    //TODO: should this be an event?
    for (auto* player : PlayerComponent::GetAllPlayers())
    {
        if (player->IsPositionDirty())
        {
            auto [row, col] = player->GetTilePosition();
            SetTarget(row, col);
            player->ClearDirtyFlag();
        }
    }

    if (m_pCurrentState)
        m_pCurrentState->Update(*this);
}

void EnemyComponent::SetTile(int row, int col)
{
    TileManager::GetInstance().RemoveEnemy(m_Row, m_Col, GetOwner());

    m_Row = row;
    m_Col = col;

    constexpr int TILE_SIZE = 48;
    GetOwner()->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);

    TileManager::GetInstance().RegisterEnemy(m_Row, m_Col, GetOwner());
}

void EnemyComponent::MoveBy(int dr, int dc)
{
    m_LastDr = dr;
    m_LastDc = dc;
    SetTile(m_Row + dr, m_Col + dc);
}

void EnemyComponent::SetTarget(int row, int col) 
{ 
    m_TargetRow = row; 
    m_TargetCol = col; 
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
    // Directions: down, up, right, left
    static const int dirs[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    int bestDr = 0, bestDc = 0;
    int bestDist = std::numeric_limits<int>::max();
    bool foundValid = false;

    for (const auto& d : dirs)
    {
        int dr = d[0], dc = d[1];
        int nr = m_Row + dr;
        int nc = m_Col + dc;

        // Skip backtracking unless it's the only option
        if (dr == -m_LastDr && dc == -m_LastDc)
            continue;

        auto tile = TileManager::GetInstance().GetTile(nr, nc);
        if (!tile || (tunnelsOnly && !tile->IsDug()))
            continue;

        int dist = std::abs(m_TargetRow - nr) + std::abs(m_TargetCol - nc);
        if (dist < bestDist)
        {
            bestDist = dist;
            bestDr = dr;
            bestDc = dc;
            foundValid = true;
        }
    }

    // Fallback: allow backtracking if no other move was valid
    if (!foundValid)
    {
        for (const auto& d : dirs)
        {
            int dr = d[0], dc = d[1];
            if (dr != -m_LastDr || dc != -m_LastDc)
                continue;

            int nr = m_Row + dr;
            int nc = m_Col + dc;

            auto tile = TileManager::GetInstance().GetTile(nr, nc);
            if (!tile || (tunnelsOnly && !tile->IsDug()))
                continue;

            return { dr, dc };
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
