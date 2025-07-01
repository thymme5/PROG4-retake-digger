#pragma once
#include "Component.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>


class TileComponent;

enum class EdgeEnemyType { Ugg, Wrongway };

struct EnemyJumpData
{
    glm::vec3 startPos{};
    glm::vec3 endPos{};
    float elapsed{ 0.f };
    float duration{ 0.9f };
    bool isJumping{ false };
    float waitTimer{ 0.f };
    float waitDuration{ 0.6f };
};

class EdgeEnemyComponent : public dae::Component
{
public:
    EdgeEnemyComponent(EdgeEnemyType type, dae::GameObject& owner);

    void Update() override;
    void Render() const override;

    void SetTileMap(const std::vector<std::vector<std::shared_ptr<TileComponent>>>& tileMap);
    void SetCurrentTile(std::shared_ptr<TileComponent> tile);

private:
    void TryMove();
    void StartJumpTo(std::shared_ptr<TileComponent> targetTile);

    EdgeEnemyType m_Type;
    std::shared_ptr<TileComponent> m_CurrentTile;
    const std::vector<std::vector<std::shared_ptr<TileComponent>>>* m_pTileMap{};

    float m_xOffset{ 0.f };
    float m_yOffset{ 0.f };

    EnemyJumpData m_Jump;
};
