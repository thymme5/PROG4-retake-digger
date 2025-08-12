#include "GoldBagComponent.h"
#include "GameObject.h"
#include "SubjectComponent.h"
#include "Observer.h"
#include "TextureComponent.h"
#include "TileManager.h"
#include "EnemyComponent.h"
#include "Timer.h"

GoldBagComponent::GoldBagComponent(dae::GameObject& owner, int row, int col)
    : InteractableComponent(owner), m_Row(row), m_Col(col)
{
    SetState(std::make_unique<IdleState>());
}

void GoldBagComponent::Update()
{
    float deltaTime = Timer::GetDeltaTime();

    if (m_IsMidFall)
    {
        glm::vec2 currentPos = GetOwner()->GetLocalPosition();
        glm::vec2 direction = m_TargetPosition - currentPos;

        if (glm::length(direction) < 1.f)
        {
            GetOwner()->SetLocalPosition(m_TargetPosition.x, m_TargetPosition.y);
            m_IsMidFall = false;

            // Re-register on new tile
            TileManager::GetInstance().RegisterInteractable(m_Row, m_Col, GetOwner());
        }
        else
        {
            glm::vec2 velocity = glm::normalize(direction) * m_FallSpeed;
            glm::vec2 newPos = currentPos + velocity * deltaTime;
            GetOwner()->SetLocalPosition(newPos.x, newPos.y);
        }
    }

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

        TileManager::GetInstance().RemoveInteractable(m_Row, m_Col, GetOwner());

        GetOwner()->Destroy();
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
bool GoldBagComponent::IsMidFall() const
{
	return m_IsMidFall;
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
    const int newRow = m_Row + 1;

    auto tileBelow = TileManager::GetInstance().GetTile(newRow, m_Col);
    if (!tileBelow || tileBelow->IsDug())
    {
        const auto enemies = TileManager::GetInstance().GetEnemiesAt(newRow, m_Col);
        for (auto* go : enemies)
        {
            if (!go) continue;
            if (auto* subj = GetOwner()->GetComponent<dae::SubjectComponent>())
                subj->Notify(dae::Event::EnemyKilled, go);
            go->Destroy();
        }

        TileManager::GetInstance().RemoveInteractable(m_Row, m_Col, GetOwner());

        ++m_Row;
        ++m_FallDistance;

        m_TargetPosition = { m_Col * TILE_SIZE, m_Row * TILE_SIZE };
        m_IsMidFall = true;
    }
}
