#include "PlayerComponent.h"
#include "GameObject.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "TextureComponent.h"

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
    if (tile) // valid move
    {
        m_Row = newRow;
        m_Col = newCol;

        std::cout << "position before change: " << m_Col * TILE_SIZE << ", " << m_Row * TILE_SIZE << std::endl;;

        GetOwner()->SetLocalPosition(m_Col * TILE_SIZE, m_Row * TILE_SIZE);
        std::cout << "position after change " << GetOwner()->GetWorldPosition().x << ", " << GetOwner()->GetWorldPosition().y << std::endl;

        DigCurrentTile();
    }
}

void PlayerComponent::DigCurrentTile()
{
    auto tile = TileManager::GetInstance().GetTile(m_Row, m_Col);
    if (tile && !tile->IsDug())
    {
        tile->SetDug(true);
        tile->GetGameObject()->GetComponent<dae::TextureComponent>()->SetVisible(false);
        TileManager::GetInstance().OnNotify(dae::Event::TileDug, GetOwner());
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