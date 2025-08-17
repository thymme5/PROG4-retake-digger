#include "EmeraldComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "PlayerComponent.h"

EmeraldComponent::EmeraldComponent(dae::GameObject& owner, int row, int col)
    : InteractableComponent(owner), m_Row(row), m_Col(col)
{

}

void EmeraldComponent::Update()
{

}

void EmeraldComponent::Interact(dae::GameObject& interactor)
{
    if (m_IsCollected)
        return;

    m_IsCollected = true;
	if (auto* player = interactor.GetComponent<PlayerComponent>())
	{
		player->Notify(dae::Event::EmeraldCollected, &interactor);
	}

    TileManager::GetInstance().RemoveInteractable(m_Row, m_Col, GetOwner());

    GetOwner()->Destroy();
}
