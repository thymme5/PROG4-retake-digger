#pragma once
#include "InteractableComponent.h"
#include "GoldBagStates.h"

#include <memory>

class GoldBagState;

constexpr int TILE_SIZE = 48;

class GoldBagComponent final : public InteractableComponent
{
public:
    GoldBagComponent(dae::GameObject& owner, int row, int col);
    void Update() override;
    void Render() const override {}

    void Interact(dae::GameObject& interactor) override;

    void SetState(std::unique_ptr<GoldBagState> newState);

    void Fall();
    int GetFallDistance() const { return m_FallDistance; }
    void ResetFallDistance() { m_FallDistance = 0; }

    int GetRow() const { return m_Row; }
    int GetCol() const { return m_Col; }



private:
    int m_Row{};
    int m_Col{};
    int m_FallDistance{ 0 };

    std::unique_ptr<GoldBagState> m_pCurrentState;
};
