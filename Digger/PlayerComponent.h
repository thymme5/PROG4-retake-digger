#pragma once
#include "PlayerState.h"
#include "Component.h"
#include "Observer.h"
#include "glm.hpp"
#include "FireballComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SubjectComponent.h"
#include "DeadState.h"

#include <memory>
#include <vector>
#include <utility>

class PlayerComponent final : public dae::Component
{
public:
    static const std::vector<PlayerComponent*>& GetAllPlayers();

    PlayerComponent(dae::GameObject& owner, int startRow, int startCol);
    ~PlayerComponent();
    void Update() override;
    void Render() const override {}

    int GetRow() const { return m_Row; }
    int GetCol() const { return m_Col; }
    std::pair<int, int> GetTilePosition() const { return { m_Row, m_Col }; }
    std::pair<int, int> GetSpawnPosition() const { return { m_SpawnRow, m_SpawnCol }; }

    bool IsPositionDirty() const { return m_PositionDirty; }
    void ClearDirtyFlag() { m_PositionDirty = false; }

    void Move(int dRow, int dCol); // Move(1, 0) to go down
    void SetTilePosition(int row, int col);

    void DigCurrentTile();

    void SetState(std::unique_ptr<PlayerState> newState);

    void ShootFireball();

    // death logic
    void MarkAsDead();
    bool IsInDeadState() const;
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

    // dirty positions for enemies
	bool m_PositionDirty{ false };  

	// Spawn location
    const int m_SpawnRow; 
    const int m_SpawnCol;

    // Fireball logic
    float m_HasFireballTimer = 0.f;
    bool m_HasFireball{ true };
    const float m_HasFireballInterval = 10.f; // Time before fireball can be used again

    // Emeralds logic
    int m_EmeraldStreak = 0;
    const int m_EmeraldStreakGoal = 8;
};
