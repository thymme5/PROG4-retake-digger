#pragma once
#include "InteractableComponent.h"
#include "TileManager.h"

class EmeraldComponent final : public InteractableComponent
{
public:
    EmeraldComponent(dae::GameObject& owner, int row, int col);

    void Update() override;
    void Render() const override {}

    void Interact(dae::GameObject& interactor) override;

    bool IsCollected() const { return m_IsCollected; }

private:
    int m_Row{};
    int m_Col{};
    bool m_IsCollected{ false };
};
