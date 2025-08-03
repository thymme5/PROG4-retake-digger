#include "PlayerComponent.h"
#include "GameObject.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "TextureComponent.h"
#include "EmeraldComponent.h"
#include "SceneManager.h"
#include "InteractableComponent.h"

constexpr int TILE_SIZE = 48;

PlayerComponent::PlayerComponent(dae::GameObject& owner, int startRow, int startCol)
    : Component(owner), m_Row{ startRow }, m_Col{ startCol }
{
    owner.SetLocalPosition(m_Col * TILE_SIZE, m_Row * TILE_SIZE);
    DigCurrentTile(); // Dig starting tile
}

void PlayerComponent::Update()
{
    if (m_pCurrentState)
        m_pCurrentState->Update(*this);
}

void PlayerComponent::Move(int dRow, int dCol)
{
    int newRow = m_Row + dRow;
    int newCol = m_Col + dCol;

    auto tile = TileManager::GetInstance().GetTile(newRow, newCol);
    if (tile)
    {
        m_Row = newRow;
        m_Col = newCol;

        GetOwner()->SetLocalPosition(m_Col * TILE_SIZE, m_Row * TILE_SIZE);

        DigCurrentTile();
    }
}

void PlayerComponent::DigCurrentTile()
{
    auto tile = TileManager::GetInstance().GetTile(m_Row, m_Col);
    if (!tile) return;

    auto* tileGO = tile->GetGameObject();

    if (tile && !tile->IsDug())
    {
        tile->SetDug(true);
        tile->GetGameObject()->GetComponent<dae::TextureComponent>()->SetVisible(false);
        TileManager::GetInstance().OnNotify(dae::Event::TileDug, GetOwner());
    }

    auto interactables = TileManager::GetInstance().GetInteractablesAt(m_Row, m_Col);
    for (const auto& interactable : interactables)
    {
        if (auto* interactableComp = interactable->GetComponent<InteractableComponent>())
        {
            interactableComp->Interact(*GetOwner());
        }
    }
}

void PlayerComponent::SetState(std::unique_ptr<PlayerState> newState)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);

    m_pCurrentState = std::move(newState);

    if (m_pCurrentState)
        m_pCurrentState->Enter(*this);
}