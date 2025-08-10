#include "GoldBagComponent.h"
#include "GameObject.h"
#include "SubjectComponent.h"
#include "Observer.h"
#include "TextureComponent.h"
#include "TileManager.h"

GoldBagComponent::GoldBagComponent(dae::GameObject& owner, int row, int col)
    : InteractableComponent(owner), m_Row(row), m_Col(col)
{
    SetState(std::make_unique<IdleState>());
}

void GoldBagComponent::Update()
{
    if (m_pCurrentState)
        m_pCurrentState->Update(*this);
}
void GoldBagComponent::Interact(dae::GameObject& interactor)
{
    if (dynamic_cast<BrokenState*>(m_pCurrentState.get()))
    {
        if (auto* subject = interactor.GetComponent<dae::SubjectComponent>())
        {
            subject->Notify(dae::Event::GoldCollected, &interactor);
        }

        //TODO: remove from scene? 
        GetOwner()->GetComponent<dae::TextureComponent>()->SetVisible(false);
    }
}

void GoldBagComponent::SetState(std::unique_ptr<GoldBagState> newState)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);

    m_pCurrentState = std::move(newState);

    if (m_pCurrentState)
        m_pCurrentState->Enter(*this);
}
GoldBagState* GoldBagComponent::GetState() const
{
	return m_pCurrentState.get();
}

bool GoldBagComponent::IsBroken() const
{
    return dynamic_cast<BrokenState*>(m_pCurrentState.get()) != nullptr;
}

bool GoldBagComponent::TryPush(int targetRow, int targetCol)
{
    auto targetTile = TileManager::GetInstance().GetTile(targetRow, targetCol);

    if (targetTile && targetTile->IsDug() &&
        TileManager::GetInstance().GetInteractablesAt(targetRow, targetCol).empty())
    {
        TileManager::GetInstance().RemoveInteractable(m_Row, m_Col, GetOwner());

        m_Row = targetRow;
        m_Col = targetCol;
        GetOwner()->SetLocalPosition(m_Col * TILE_SIZE, m_Row * TILE_SIZE);

        TileManager::GetInstance().RegisterInteractable(m_Row, m_Col, GetOwner());
        return true;
    }

    return false;
}

void GoldBagComponent::Fall()
{
    int newRow = m_Row + 1;

    auto tileBelow = TileManager::GetInstance().GetTile(newRow, m_Col);
    if (!tileBelow || tileBelow->IsDug())
    {
        // Remove from current tile
        TileManager::GetInstance().RemoveInteractable(m_Row, m_Col, GetOwner());

        ++m_Row;
        ++m_FallDistance;

        // Move GameObject visually
        GetOwner()->SetLocalPosition(m_Col * TILE_SIZE, m_Row * TILE_SIZE);

        // Re-register on new tile
        TileManager::GetInstance().RegisterInteractable(m_Row, m_Col, GetOwner());
    }
}
