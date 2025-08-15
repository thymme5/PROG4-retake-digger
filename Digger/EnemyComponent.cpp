#include "EnemyComponent.h"
#include "EnemyState.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "GameObject.h"
#include "NobbinState.h" 
#include "PlayerComponent.h"
#include "NobbinControlState.h"

#include <queue>
#include <set>
#include <algorithm>

constexpr int TILE_SIZE = 48;
static int s_EnemyIDCounter = 0;

// --- tiny registry of all enemies ---
static std::vector<EnemyComponent*>& EnemyList() {
    static std::vector<EnemyComponent*> s;
    return s;
}

EnemyComponent::EnemyComponent(dae::GameObject& owner, int startRow, int startCol, bool isPlayerControlled)
    : Component(owner), m_Row(startRow), m_Col(startCol), m_IsPlayerControlled(isPlayerControlled)
{
    m_EnemyID = s_EnemyIDCounter++;
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
    auto it = std::find(v.begin(), v.end(), this);
    if (it != v.end()) {
        v.erase(it);
    }

    TileManager::GetInstance().RemoveEnemy(m_Row, m_Col, GetOwner());
}

void EnemyComponent::Update()
{
    if (!m_IsPlayerControlled && !m_IsMoving)
    {
        PlayerComponent* closestPlayer = nullptr;
        float closestDistance = std::numeric_limits<float>::max();

        for (auto* player : PlayerComponent::GetAllPlayers())
        {
            if (player->IsInDeadState()) continue;

            auto [playerRow, playerCol] = player->GetTilePosition();
            float distance = std::abs(playerRow - m_Row) + std::abs(playerCol - m_Col); // Manhattan distance

            if (distance < closestDistance) {
                closestDistance = distance;
                closestPlayer = player;
            }
        }

        // Set target to closest player
        if (closestPlayer) {
            auto [row, col] = closestPlayer->GetTilePosition();
            SetTarget(row, col);
        }

        // Check for collision with any player
        for (auto* player : PlayerComponent::GetAllPlayers())
        {
            if (GetTilePosition() == player->GetTilePosition() && !player->IsInDeadState())
            {
                player->MarkAsDead();
            }
        }
    }

    // Movement interpolation
    if (m_IsMoving)
    {
        glm::vec2 currentPos = GetOwner()->GetWorldPosition();
        glm::vec2 targetPos = { m_TargetCol * TILE_SIZE, m_TargetRow * TILE_SIZE };
        glm::vec2 newPos = currentPos + m_MoveDirection;

        bool reached = false;

        if (std::abs(m_MoveDirection.x) > 0.001f) {
            if ((m_MoveDirection.x > 0.f && newPos.x >= targetPos.x) ||
                (m_MoveDirection.x < 0.f && newPos.x <= targetPos.x)) {
                reached = true;
                newPos.x = targetPos.x; 
            }
        }

        if (std::abs(m_MoveDirection.y) > 0.001f) {
            if ((m_MoveDirection.y > 0.f && newPos.y >= targetPos.y) ||
                (m_MoveDirection.y < 0.f && newPos.y <= targetPos.y)) {
                reached = true;
                newPos.y = targetPos.y;
            }
        }

        if (reached)
        {
            TileManager::GetInstance().RemoveEnemy(m_Row, m_Col, GetOwner());

            m_Row = m_TargetRow;
            m_Col = m_TargetCol;
            m_IsMoving = false;

            TileManager::GetInstance().RegisterEnemy(m_Row, m_Col, GetOwner());
            GetOwner()->SetLocalPosition(targetPos.x, targetPos.y);
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

    m_IsMoving = false;

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
    // Already at target position
    if (m_Row == m_TargetRow && m_Col == m_TargetCol) 
    {
        return { 0, 0 };
    }

    struct Node 
    {
        int row, col, dr, dc;
        int distance;
    };

    auto compare = [this](const Node& a, const Node& b) {
        int distA = std::abs(a.row - m_TargetRow) + std::abs(a.col - m_TargetCol);
        int distB = std::abs(b.row - m_TargetRow) + std::abs(b.col - m_TargetCol);
        return distA > distB; // Min-heap
        };

    std::priority_queue<Node, std::vector<Node>, decltype(compare)> pq(compare);
    std::set<std::pair<int, int>> visited;

    static const int dirs[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

    visited.insert({ m_Row, m_Col });

    for (const auto& d : dirs)
    {
        int nr = m_Row + d[0];
        int nc = m_Col + d[1];

        if (nr < 0 || nc < 0 ||
            nr >= TileManager::GetInstance().GetHeight() ||
            nc >= TileManager::GetInstance().GetWidth()) continue;

        auto tile = TileManager::GetInstance().GetTile(nr, nc);
        if (!tile || (tunnelsOnly && !tile->IsDug())) continue;

        if (visited.count({ nr, nc })) continue;

        int dist = std::abs(nr - m_TargetRow) + std::abs(nc - m_TargetCol);
        pq.push({ nr, nc, d[0], d[1], dist });
        visited.insert({ nr, nc });
    }

    while (!pq.empty())
    {
        auto node = pq.top();
        pq.pop();

        if (node.row == m_TargetRow && node.col == m_TargetCol)
            return { node.dr, node.dc };

        for (const auto& d : dirs)
        {
            int nr = node.row + d[0];
            int nc = node.col + d[1];

            if (nr < 0 || nc < 0 ||
                nr >= TileManager::GetInstance().GetHeight() ||
                nc >= TileManager::GetInstance().GetWidth()) continue;

            if (visited.count({ nr, nc })) continue;

            auto tile = TileManager::GetInstance().GetTile(nr, nc);
            if (!tile || (tunnelsOnly && !tile->IsDug())) continue;

            int dist = std::abs(nr - m_TargetRow) + std::abs(nc - m_TargetCol);
            pq.push({ nr, nc, node.dr, node.dc, dist });
            visited.insert({ nr, nc });
        }
    }

    return { 0, 0 };
}
