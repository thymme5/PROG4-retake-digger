#pragma once
#include "PlayerState.h"
#include "Component.h"
#include "Observer.h"
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
};
