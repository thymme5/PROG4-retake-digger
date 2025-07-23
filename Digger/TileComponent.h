#pragma once
#include "Component.h"

class TileComponent final : public dae::Component
{
public:
    TileComponent(dae::GameObject& owner, int row, int col);

    int GetRow() const;
    int GetCol() const;

    void SetDug(bool isDug);
    bool IsDug() const;

    void SetHasEmerald(bool hasEmerald);
    bool HasEmerald() const;

    void SetHasGoldBag(bool hasGoldBag);
    bool HasGoldBag() const;

    void Update() override {}
    void Render() const override {}

    dae::GameObject* GetGameObject() const;

private:
    int m_Row{};
    int m_Col{};
    bool m_IsDug{};
    bool m_HasEmerald{};
    bool m_HasGoldBag{};
};
