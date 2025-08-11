#include "PlayerComponent.h"
#include "GameObject.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "TextureComponent.h"
#include "EmeraldComponent.h"
#include "SceneManager.h"
#include "InteractableComponent.h"
#include "GoldBagComponent.h"

static std::vector<PlayerComponent*>& PlayerList()
{
    static std::vector<PlayerComponent*> s;
    return s;
}

const std::vector<PlayerComponent*>& PlayerComponent::GetAllPlayers()
{
    return PlayerList();
}

PlayerComponent::PlayerComponent(dae::GameObject& owner, int startRow, int startCol)
    : Component(owner), m_Row{ startRow }, m_Col{ startCol }
{
    owner.SetLocalPosition(m_Col * TILE_SIZE, m_Row * TILE_SIZE);
    PlayerList().push_back(this);

    DigCurrentTile(); // Dig starting tile 
}
PlayerComponent::~PlayerComponent()
{
    auto& list = PlayerList();
    auto it = std::find(list.begin(), list.end(), this);
    if (it != list.end())
    {
        list.erase(it);
    }
}

void PlayerComponent::Update()
{
    if (m_IsMoving)
    {
        glm::vec2 currentPos = GetOwner()->GetWorldPosition();
        glm::vec2 targetPos = { m_TargetCol * TILE_SIZE, m_TargetRow * TILE_SIZE };

        glm::vec2 newPos = currentPos + m_MoveDirection;

        // Overshoot clamp
        bool reached = false;
        if (m_MoveDirection.x > 0.f && newPos.x >= targetPos.x) reached = true;
        else if (m_MoveDirection.x < 0.f && newPos.x <= targetPos.x) reached = true;
        else if (m_MoveDirection.y > 0.f && newPos.y >= targetPos.y) reached = true;
        else if (m_MoveDirection.y < 0.f && newPos.y <= targetPos.y) reached = true;

        if (reached)
        {
            GetOwner()->SetLocalPosition(targetPos.x, targetPos.y);
            m_Row = m_TargetRow;
            m_Col = m_TargetCol;
            m_IsMoving = false;
            m_PositionDirty = true;
            DigCurrentTile();
        }
        else
        {
            GetOwner()->SetLocalPosition(newPos.x, newPos.y);
        }
    }

    if (m_pCurrentState)
        m_pCurrentState->Update(*this);
}

void PlayerComponent::Move(int dRow, int dCol)
{
    if (m_IsMoving) return;

    m_LastDirRow = dRow;
    m_LastDirCol = dCol;

    const int newRow = m_Row + dRow;
    const int newCol = m_Col + dCol;

    auto tile = TileManager::GetInstance().GetTile(newRow, newCol);
    if (!tile) return;

    // Check for gold bag
    for (auto* obj : TileManager::GetInstance().GetInteractablesAt(newRow, newCol))
    {
        if (auto* goldBag = obj->GetComponent<GoldBagComponent>())
        {
            // check if bag can be pushed 
            if (goldBag->IsBroken()) continue;

            // Only allow pushing left/right
            if (dRow != 0) return;

            const int bagRow = newRow;
            const int bagCol = newCol;

            const int pushCol = bagCol + dCol;
            const int pushRow = bagRow;

            // Ensure player is on correct side of bag
            const int playerCol = m_Col;
            if ((dCol == -1 && playerCol != bagCol + 1) || // player must be to the right
                (dCol == 1 && playerCol != bagCol - 1))    // player must be to the left
            {
                return;
            }

            auto pushTile = TileManager::GetInstance().GetTile(pushRow, pushCol);
            if (!pushTile || !pushTile->IsDug()) return;

            if (!TileManager::GetInstance().GetInteractablesAt(pushRow, pushCol).empty()) return;

            if (!goldBag->TryPush(pushRow, pushCol)) return;
        }
    }

    // No obstruction or bag was pushed successfully
    m_TargetRow = newRow;
    m_TargetCol = newCol;
    m_MoveDirection = glm::vec2{
        static_cast<float>(dCol) * m_MoveSpeedPerFrame,
        static_cast<float>(dRow) * m_MoveSpeedPerFrame
    };
    m_IsMoving = true;
}


void PlayerComponent::DigCurrentTile()
{
    auto tile = TileManager::GetInstance().GetTile(m_Row, m_Col);
    if (!tile) return;

    auto* tileGO = tile->GetGameObject();

    if (tile && !tile->IsDug())
    {
        tile->SetDug(true);
        tile->GetGameObject()->Destroy();
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

void PlayerComponent::ShootFireball()
{
    if (m_HasFireball) return; 

    auto fireball = std::make_unique<dae::GameObject>();
    fireball->SetLocalPosition(m_Col * TILE_SIZE, m_Row * TILE_SIZE);

    fireball->AddComponent<dae::TextureComponent>(*fireball, "fireball.png", 1.f);
    fireball->AddComponent<FireballComponent>(*fireball, m_Row, m_Col, m_LastDirRow, m_LastDirCol);

    dae::SceneManager::GetInstance().GetActiveScene().Add(std::move(fireball));

    m_HasFireball = true;
}



void PlayerComponent::SetState(std::unique_ptr<PlayerState> newState)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);

    m_pCurrentState = std::move(newState);

    if (m_pCurrentState)
        m_pCurrentState->Enter(*this);
}

