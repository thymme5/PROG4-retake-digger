#include "EdgeEnemyComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "TileComponent.h"
#include "Time.h"

EdgeEnemyComponent::EdgeEnemyComponent(EdgeEnemyType type, dae::GameObject& owner)
    : Component(owner), m_Type(type)
{
    GetOwner()->SetRenderLayer(RenderLayer::Characters);
}

void EdgeEnemyComponent::SetTileMap(const std::vector<std::vector<std::shared_ptr<TileComponent>>>& tileMap)
{
    m_pTileMap = &tileMap;

    if (!m_pTileMap || m_pTileMap->empty()) return;

    // Bottom-left for Ugg, bottom-right for Wrongway
    m_CurrentTile = (m_Type == EdgeEnemyType::Ugg)
        ? (*m_pTileMap).back().front()
        : (*m_pTileMap).back().back();

    if (m_CurrentTile)
    {
        auto& pos = m_CurrentTile->GetOwner()->GetTransform().GetPosition();
        GetOwner()->SetPosition(pos.x + m_xOffset, pos.y + m_yOffset);
    }
}

void EdgeEnemyComponent::SetCurrentTile(std::shared_ptr<TileComponent> tile)
{
    m_CurrentTile = std::move(tile);

    if (m_CurrentTile)
    {
        const auto& pos = m_CurrentTile->GetOwner()->GetTransform().GetPosition();
        GetOwner()->SetPosition(pos.x + m_xOffset, pos.y + m_yOffset);
    }
}

void EdgeEnemyComponent::Update()
{
    constexpr float fixedDeltaTime = 1.f / 60.f;

    if (m_Jump.isJumping)
    {
        m_Jump.elapsed += fixedDeltaTime;

        float t = m_Jump.elapsed / m_Jump.duration;
        if (t >= 1.f)
        {
            GetOwner()->SetPosition(m_Jump.endPos.x, m_Jump.endPos.y);
            m_Jump.isJumping = false;
            m_Jump.waitTimer = 0.f;
            return;
        }
        else
        {
            glm::vec2 pos = glm::mix(
                glm::vec2(m_Jump.startPos.x, m_Jump.startPos.y),
                glm::vec2(m_Jump.endPos.x, m_Jump.endPos.y),
                t
            );
            const float arcHeight = 10.f;
            pos.y -= sin(t * 3.14f * arcHeight);
            GetOwner()->SetPosition(pos.x, pos.y);
            return;
        }
    }

    if (m_Jump.waitTimer < m_Jump.waitDuration)
    {
        m_Jump.waitTimer += fixedDeltaTime;
        return;
    }

    TryMove();
}
void EdgeEnemyComponent::Render() const
{

}
void EdgeEnemyComponent::TryMove()
{
    if (!m_CurrentTile || !m_pTileMap) return;

    auto [row, col] = m_CurrentTile->GetGridPosition();
    int newRow = row - 1;
    int newCol = col;

    // Diagonal movement depends on type
    newCol += (m_Type == EdgeEnemyType::Ugg) ? 1 : -1;

    // Bounds check
    if (newRow < 0 || newRow >= static_cast<int>(m_pTileMap->size())) return;
    if (newCol < 0 || newCol >= static_cast<int>((*m_pTileMap)[newRow].size())) return;

    auto targetTile = (*m_pTileMap)[newRow][newCol];
    if (!targetTile) return;

    StartJumpTo(targetTile);
}

void EdgeEnemyComponent::StartJumpTo(std::shared_ptr<TileComponent> targetTile)
{
    m_CurrentTile = std::move(targetTile);

    const glm::vec3 startPos = GetOwner()->GetTransform().GetPosition();
    const glm::vec3 endPos = m_CurrentTile->GetOwner()->GetTransform().GetPosition() + glm::vec3(m_xOffset, m_yOffset, 0.f);

    m_Jump.startPos = startPos;
    m_Jump.endPos = endPos;
    m_Jump.elapsed = 0.f;
    m_Jump.duration = 0.4f;
    m_Jump.isJumping = true;
}
