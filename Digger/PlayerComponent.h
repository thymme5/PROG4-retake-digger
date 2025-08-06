#pragma once
#include "PlayerState.h"
#include "Component.h"
#include "Observer.h"
#include "glm.hpp"
#include "FireballComponent.h"
#include "SceneManager.h"
#include "Scene.h"

#include <memory>

class PlayerComponent final : public dae::Component
{
public:
    PlayerComponent(dae::GameObject& owner, int startRow, int startCol);

    void Update() override;
    void Render() const override {}

    int GetRow() const { return m_Row; }
    int GetCol() const { return m_Col; }

    void Move(int dRow, int dCol); // Move(1, 0) to go down
    void DigCurrentTile();

    void SetState(std::unique_ptr<PlayerState> newState);

    void ShootFireball();

private:
    int m_Row{};
    int m_Col{};
    std::unique_ptr<PlayerState> m_pCurrentState;

    // movement related variables
    int m_TargetRow{}, m_TargetCol{};
    glm::vec2 m_MoveDirection{ 0.f, 0.f };
    float m_Speed{ 100.f }; //pixels per second
    bool m_IsMoving{ false };
    float m_MoveSpeedPerFrame{ .05f };

    // last direction in which player moved for fireball's purpose
    int m_LastDirRow{ 0 };
    int m_LastDirCol{ 1 }; // facing right on default
    bool m_HasFireball{ false };
};
