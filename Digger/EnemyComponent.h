#pragma once
#include "Component.h"
#include <memory>
#include <utility> 
#include "SubjectComponent.h"
#include <glm/vec2.hpp>

class EnemyState; 

class EnemyComponent final : public dae::Component
{
public:
    EnemyComponent(dae::GameObject& owner, int startRow, int startCol, bool isPlayerControlled = false);
    ~EnemyComponent() override;

    void Update() override;
    void Render() const override {}

    // Row/Col position in the tile grid
    int GetRow() const { return m_Row; }
    int GetCol() const { return m_Col; }
    std::pair<int, int> GetTilePosition() const { return { m_Row, m_Col }; }
    void SetTile(int row, int col);

    // Move by row/col offset
    void MoveBy(int dr, int dc);

    // Target
    void SetTarget(int row, int col);

    // State handling
    void SetState(std::unique_ptr<EnemyState> newState);

    // Pathfinding helper
    std::pair<int, int> BestStepTowardTarget(bool tunnelsOnly) const;
    
    // Player controlled
    void SetPlayerControlled(bool controlled) { m_IsPlayerControlled = controlled; }
    bool IsPlayerControlled() const { return m_IsPlayerControlled; }

	bool IsMoving() const { return m_IsMoving; }
private:
    int m_EnemyID = -1;
   
    int m_Row{};
    int m_Col{};

    int m_FrameCounter{ 0 };

    std::unique_ptr<EnemyState> m_pCurrentState;

    int m_LastDr{ 0 }; // last direction moved in row
    int m_LastDc{ 0 }; // last direction moved in col

    bool m_IsPlayerControlled{ false };

    // Movement variables
    int m_TargetRow{}, m_TargetCol{};
    glm::vec2 m_MoveDirection{ 0.f, 0.f };
    bool m_IsMoving{ false };
    float m_Speed{ 70.f };
};
