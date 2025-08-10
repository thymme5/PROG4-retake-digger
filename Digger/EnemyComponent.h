#pragma once
#include "Component.h"
#include <memory>
#include <utility> // for std::pair

class EnemyState; // forward declaration

class EnemyComponent final : public dae::Component
{
public:
    EnemyComponent(dae::GameObject& owner, int startRow, int startCol);
    ~EnemyComponent() override = default;

    void Update() override;
    void Render() const override {}

    // Row/Col position in the tile grid
    int GetRow() const { return m_Row; }
    int GetCol() const { return m_Col; }
    void SetTile(int row, int col);

    // Move by row/col offset
    void MoveBy(int dr, int dc);

    // Target
    void SetTarget(int row, int col) { m_TargetRow = row; m_TargetCol = col; }

    // State handling
    void SetState(std::unique_ptr<EnemyState> newState);

    // Pathfinding helper
    std::pair<int, int> BestStepTowardTarget(bool tunnelsOnly) const;

    // slow down movement
    bool ShouldStepThisFrame(int framesPerStep);

private:
    int m_Row{};
    int m_Col{};
    int m_TargetRow{};
    int m_TargetCol{};

    int m_FrameCounter{ 0 };

    std::unique_ptr<EnemyState> m_pCurrentState;

	int m_LastDr{ 0 }; // last direction moved in row
	int m_LastDc{ 0 }; // last direction moved in col

};
