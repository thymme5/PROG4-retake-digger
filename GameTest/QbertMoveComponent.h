#pragma once
#include "Component.h"
#include "TileComponent.h"

#include <glm.hpp>
#include <memory>
#include <vector>

// ==============================
// ==============================
// ==============================
//todo: should be vector not enum
// cooked
// cooked
// cooked
// cooked
// cooked
// cooked
// cooked
// cooked
// cooked
// ==============================
// ==============================
// ==============================
enum class Direction
{
    UpLeft,
    UpRight,
    DownLeft,
    DownRight
};
// ==============================
// cooked
// cooked
// cooked
// cooked
// cooked
// cooked
// cooked
// cooked
// cooked
// ==============================

struct JumpData
{
    glm::vec2 startPos;
    glm::vec2 endPos;
    float elapsed = 0.f;
    float duration = 0.3f;
    bool isJumping = false;
};

class QbertMoveComponent : public dae::Component
{
public:
    QbertMoveComponent(dae::GameObject& pOwner);
    void Update() override;
    void Render() const override;

    void SetCurrentTile(std::shared_ptr<TileComponent> tile);
    void SetTileMap(const std::vector<std::vector<std::shared_ptr<TileComponent>>>& tileMap);

    std::shared_ptr<TileComponent> GetCurrentTile();

    void TryMove(Direction dir);

private:
    std::shared_ptr<TileComponent> m_CurrentTile;
    const std::vector<std::vector<std::shared_ptr<TileComponent>>>* m_pTileMap = nullptr;
    const float m_xOffset = 15.f; //magic number but pre-calculated value (kevin would love this)
    const float m_yOffset = -19.f; //magic number but pre-calculated value
        
    JumpData m_Jump;


};

