#include "EnemyComponent.h"
#include "EnemyState.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "GameObject.h"
#include "NobbinState.h" 
#include "PlayerComponent.h"
#include "NobbinControlState.h"

constexpr int TILE_SIZE = 48;

// --- tiny registry of all enemies ---
static std::vector<EnemyComponent*>& EnemyList() {
    static std::vector<EnemyComponent*> s;
    return s;
}

EnemyComponent::EnemyComponent(dae::GameObject& owner, int startRow, int startCol, bool isPlayerControlled)
    : Component(owner), m_Row(startRow), m_Col(startCol), m_IsPlayerControlled(isPlayerControlled)
{
    EnemyList().push_back(this);

    if (m_IsPlayerControlled)
        SetState(std::make_unique<NobbinControlState>());
    else
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
        if (GetTilePosition() == player->GetTilePosition() && !player->IsInDeadState())
        {
            player->MarkAsDead(); 
        }
    }

    // Movement interpolation
    if (m_IsMoving)
    {
        glm::vec2 currentPos = GetOwner()->GetWorldPosition();
        glm::vec2 targetPos = { m_TargetCol * TILE_SIZE, m_TargetRow * TILE_SIZE };
        glm::vec2 newPos = currentPos + m_MoveDirection;

        bool reached = false;
        if (m_MoveDirection.x > 0.f && newPos.x >= targetPos.x) reached = true;
        else if (m_MoveDirection.x < 0.f && newPos.x <= targetPos.x) reached = true;
        else if (m_MoveDirection.y > 0.f && newPos.y >= targetPos.y) reached = true;
        else if (m_MoveDirection.y < 0.f && newPos.y <= targetPos.y) reached = true;

        if (reached)
        {
            GetOwner()->SetLocalPosition(targetPos.x, targetPos.y);
            m_Row = m_TargetRow;
            m_Col = m_TargetCol;
            m_IsMoving = false;
            SetTile(m_Row, m_Col);
        }
        else
        {
            GetOwner()->SetLocalPosition(newPos.x, newPos.y);
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
    if (m_IsMoving) return;

    const int newRow = m_Row + dr;
    const int newCol = m_Col + dc;

    auto& tileManager = TileManager::GetInstance();
    if (newRow < 0 || newRow >= tileManager.GetHeight() ||
        newCol < 0 || newCol >= tileManager.GetWidth())
    {
        std::cerr << "[EnemyComponent] Invalid move: (" << newRow << ", " << newCol << ") out of bounds." << std::endl;
        return;
    }
    auto tile = tileManager.GetTile(newRow, newCol);
    if (!tile) return;

    if (m_IsPlayerControlled && !tile->IsDug())
    {
        return;
    }

    m_LastDr = dr;
    m_LastDc = dc;

    m_TargetRow = newRow;
    m_TargetCol = newCol;
    m_MoveDirection = glm::vec2{
        static_cast<float>(dc) * m_MoveSpeedPerFrame,
        static_cast<float>(dr) * m_MoveSpeedPerFrame
    };
    m_IsMoving = true;
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
