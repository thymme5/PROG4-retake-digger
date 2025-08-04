#pragma once
#include "PlayerState.h"
#include "Component.h"
#include "Observer.h"
#include "glm.hpp"

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

private:
    int m_Row{};
    int m_Col{};
    std::unique_ptr<PlayerState> m_pCurrentState;

    // Movement related variables
    int m_TargetRow{}, m_TargetCol{};
    glm::vec2 m_MoveDirection{ 0.f, 0.f };
    float m_Speed{ 100.f }; // Pixels per second
    bool m_IsMoving{ false };
    float m_MoveSpeedPerFrame{ .10f };
};
