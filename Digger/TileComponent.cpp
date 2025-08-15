#include "TileComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"

TileComponent::TileComponent(dae::GameObject& owner, int row, int col)
    : Component(owner), m_Row(row), m_Col(col)
{
}

int TileComponent::GetRow() const { return m_Row; }
int TileComponent::GetCol() const { return m_Col; }

void TileComponent::SetDug(bool isDug)
{
	if (!m_pOwner || m_pOwner->IsMarkedForDestroy())
	{
		std::cerr << "[TileComponent] ERROR: Owner is null or marked for destroy at (" << m_Row << ", " << m_Col << ")" << std::endl;
		return;
	}

	if (m_pOwner->HasComponent<dae::TextureComponent>())
	{
		m_pOwner->GetComponent<dae::TextureComponent>()->SetVisible(!isDug);
	}
    m_IsDug = isDug;
}
bool TileComponent::IsDug() const { return m_IsDug; }

void TileComponent::SetHasEmerald(bool hasEmerald) { m_HasEmerald = hasEmerald; }
bool TileComponent::HasEmerald() const { return m_HasEmerald; }

void TileComponent::SetHasGoldBag(bool hasGoldBag) { m_HasGoldBag = hasGoldBag; }
bool TileComponent::HasGoldBag() const { return m_HasGoldBag; }

dae::GameObject* TileComponent::GetGameObject() const { return m_pOwner; }