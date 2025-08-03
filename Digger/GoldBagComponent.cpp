#include "GoldBagComponent.h"
#include "GameObject.h"

GoldBagComponent::GoldBagComponent(dae::GameObject& owner, int row, int col)
    : Component(owner), m_Row(row), m_Col(col)
{
    SetState(std::make_unique<IdleState>());
}

void GoldBagComponent::Update()
{
    if (m_pCurrentState)
        m_pCurrentState->Update(*this);
}

void GoldBagComponent::SetState(std::unique_ptr<GoldBagState> newState)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);

    m_pCurrentState = std::move(newState);

    if (m_pCurrentState)
        m_pCurrentState->Enter(*this);
}

void GoldBagComponent::Fall()
{
    // add 1 to fall distance, can use for detecting if it breaks later
    ++m_FallDistance;

    // Move goldbag down one tile visually
    ++m_Row;

    // My sincere apologies for having to do this everywhere
    const int tileSize = 48;

    GetOwner()->SetLocalPosition(m_Col * tileSize, m_Row * tileSize);
}
