#include "FireballComponent.h"
#include "TileManager.h"
#include "GameObject.h"
#include "TileComponent.h"
#include "TextureComponent.h"
#include "EnemyComponent.h"
#include "Timer.h"
#include <iostream>

constexpr int TILE_SIZE = 48;

FireballComponent::FireballComponent(dae::GameObject& owner, int row, int col, int dRow, int dCol)
    : Component(owner), m_Row{ row }, m_Col{ col }, m_DirRow{ dRow }, m_DirCol{ dCol }
{
    owner.SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);

    // Set up movement speed (pixels per second)
    m_MoveSpeed = 200.0f; // Adjust this value for fireball speed
    m_IsMoving = false;
    m_MoveTimer = 0.0f;

    std::cout << "[Fireball] Created at (" << row << ", " << col << ") direction (" << dRow << ", " << dCol << ")" << std::endl;
}

void FireballComponent::Update()
{
    const float dt = Timer::GetDeltaTime();

    if (m_IsMoving)
    {
        CheckEnemyCollision();

        // Interpolate movement
        glm::vec2 currentPos = GetOwner()->GetWorldPosition();
        glm::vec2 targetPos = { m_TargetCol * TILE_SIZE, m_TargetRow * TILE_SIZE };

        // Calculate movement direction
        glm::vec2 direction = targetPos - currentPos;
        float distance = glm::length(direction);

        if (distance < 2.0f) // Close enough to target
        {
            // Snap to exact position
            GetOwner()->SetLocalPosition(targetPos.x, targetPos.y);
            m_Row = m_TargetRow;
            m_Col = m_TargetCol;
            m_IsMoving = false;
        }
        else
        {
            // Move toward target
            glm::vec2 normalizedDir = glm::normalize(direction);
            glm::vec2 velocity = normalizedDir * m_MoveSpeed * dt;
            glm::vec2 newPos = currentPos + velocity;
            GetOwner()->SetLocalPosition(newPos.x, newPos.y);
        }
    }
    else
    {
        // Time to start next movement
        m_MoveTimer += dt;
        const float MOVE_INTERVAL = 0.1f; // Slight delay between tile movements

        if (m_MoveTimer >= MOVE_INTERVAL)
        {
            m_MoveTimer = 0.0f;
            StartNextMovement();
        }
    }
}

void FireballComponent::StartNextMovement()
{
    int nextRow = m_Row + m_DirRow;
    int nextCol = m_Col + m_DirCol;

    std::cout << "[Fireball] Trying to move from (" << m_Row << ", " << m_Col << ") to (" << nextRow << ", " << nextCol << ")" << std::endl;

    // Bounds checking
    auto& tileManager = TileManager::GetInstance();
    if (nextRow < 0 || nextRow >= tileManager.GetHeight() ||
        nextCol < 0 || nextCol >= tileManager.GetWidth())
    {
        std::cout << "[Fireball] Out of bounds, destroying" << std::endl;
        GetOwner()->Destroy();
        return;
    }

    auto tile = tileManager.GetTile(nextRow, nextCol);
    if (!tile)
    {
        std::cout << "[Fireball] No tile found, destroying" << std::endl;
        GetOwner()->Destroy();
        return;
    }

    // Check if tile is dug (fireball can only travel through tunnels)
    if (!tile->IsDug())
    {
        std::cout << "[Fireball] Hit wall at (" << nextRow << ", " << nextCol << "), destroying" << std::endl;
        GetOwner()->Destroy();
        return;
    }

    // Start movement to next tile
    m_TargetRow = nextRow;
    m_TargetCol = nextCol;
    m_IsMoving = true;

    std::cout << "[Fireball] Started movement to (" << m_TargetRow << ", " << m_TargetCol << ")" << std::endl;
}

void FireballComponent::CheckEnemyCollision()
{
    glm::vec2 fireballPos = GetOwner()->GetWorldPosition();
    const float COLLISION_RADIUS = 24.0f;

    auto& tileManager = TileManager::GetInstance();
    auto allEnemies = tileManager.GetAllEnemies();

    for (auto* obj : allEnemies)
    {
        if (!obj->HasComponent<EnemyComponent>()) continue;

        glm::vec2 enemyPos = obj->GetWorldPosition();
        float dist = glm::length(fireballPos - enemyPos);

        if (dist < COLLISION_RADIUS)
        {
            std::cout << "[Fireball] Hit enemy at distance " << dist << ", both destroyed" << std::endl;
            obj->Destroy();
            GetOwner()->Destroy();
            return;
        }
    }
}

void FireballComponent::Render() const
{

}