#include "PlayerComponent.h"
#include "GameObject.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "TextureComponent.h"
#include "EmeraldComponent.h"
#include "SceneManager.h"
#include "InteractableComponent.h"
#include "GoldBagComponent.h"
#include "Timer.h"

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
	: Component(owner), m_Row{ startRow }, m_Col{ startCol }, m_SpawnCol{ startCol }, m_SpawnRow{ startRow }
{
    owner.SetLocalPosition(m_Col * TILE_SIZE, m_Row * TILE_SIZE);
    
    // Assign ID based on size of PlayerList 
    m_PlayerID = static_cast<int>(PlayerList().size());

	std::cout << "[PlayerComponent] Player ID set to " << m_PlayerID << '\n';

    PlayerList().push_back(this);

    DigCurrentTile(); // Dig starting tile 
}
PlayerComponent::~PlayerComponent()
{
    std::cout << "[PlayerComponent] Player size before deletion: " << std::to_string(PlayerList().size()) <<  "\n";
    auto& list = PlayerList();
    auto it = std::find(list.begin(), list.end(), this);
    if (it != list.end())
    {
        list.erase(it);
    }
    std::cout << "[PlayerComponent] Player " << m_PlayerID << " destroyed.\n";
    std::cout << "[PlayerComponent] Player size after deletion: " << std::to_string(PlayerList().size()) << "\n";

}
void PlayerComponent::ClearAllPlayers()
{
    PlayerList().clear();
}
void PlayerComponent::Update()
{

    if (m_IsMoving)
    {
        glm::vec2 currentPos = GetOwner()->GetWorldPosition();
        glm::vec2 targetPos = { m_TargetCol * TILE_SIZE, m_TargetRow * TILE_SIZE };

        float dt = Timer::GetDeltaTime();
        glm::vec2 step = m_MoveDirection * m_Speed * dt;
        glm::vec2 newPos = currentPos + step;
        
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

    // Fireball cooldown logic
    if (!m_HasFireball)
    {
        m_HasFireballTimer += Timer::GetDeltaTime();
        if (m_HasFireballTimer >= m_HasFireballInterval)
        {
            m_HasFireball = true;
            m_HasFireballTimer = 0.f;
        }
    }

    if (m_pCurrentState)
        m_pCurrentState->Update(*this);
}

void PlayerComponent::Move(int dRow, int dCol)
{
    if (m_IsMoving || IsInDeadState()) return;

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
    m_MoveDirection = glm::vec2{ static_cast<float>(dCol), static_cast<float>(dRow) };
    m_IsMoving = true;
}

void PlayerComponent::SetTilePosition(int row, int col)
{
    m_Row = row;
    m_Col = col;

    // Force update because I'm unsure if it will work otherwise
    GetOwner()->SetLocalPosition(m_Col * TILE_SIZE, m_Row * TILE_SIZE);
}

void PlayerComponent::DigCurrentTile()
{
    auto tile = TileManager::GetInstance().GetTile(m_Row, m_Col);
    if (!tile) return;

    auto* tileGO = tile->GetGameObject();

    if (tile && !tile->IsDug())
    {
        tile->SetDug(true);

        if (auto* playerSubj = GetOwner()->GetComponent<dae::SubjectComponent>())
        {
            playerSubj->Notify(dae::Event::TileDug, GetOwner());
        }
    }

    auto interactables = TileManager::GetInstance().GetInteractablesAt(m_Row, m_Col);
    for (const auto& interactable : interactables)
    {
        if (auto* interactableComp = interactable->GetComponent<InteractableComponent>())
        {
            interactableComp->Interact(*GetOwner());

            // Emerald streak logic
            if (interactable->GetComponent<EmeraldComponent>())
            {
                m_EmeraldStreak++;

                if (m_EmeraldStreak == m_EmeraldStreakGoal)
                {
                    m_EmeraldStreak = 0; // Reset after reaching goal

                    if (auto* playerSubj = GetOwner()->GetComponent<dae::SubjectComponent>())
                    {
                        playerSubj->Notify(dae::Event::PlayerCollected8Emeralds, GetOwner());
                    }
                }
            }
            else
            {
                m_EmeraldStreak = 0; // Break emerald streak, even if object interacted is not an emerald
            }
        }
    }
}

void PlayerComponent::ShootFireball()
{
    if (!m_HasFireball) return; 

    auto fireball = std::make_unique<dae::GameObject>();
    fireball->SetLocalPosition(m_Col * TILE_SIZE, m_Row * TILE_SIZE);

    fireball->AddComponent<dae::TextureComponent>(*fireball, "fireball.png", 1.f);
    fireball->AddComponent<FireballComponent>(*fireball, m_Row, m_Col, m_LastDirRow, m_LastDirCol);

    dae::SceneManager::GetInstance().GetActiveScene().Add(std::move(fireball));

    m_HasFireball = false;
    m_HasFireballTimer = 0.f;
}

void PlayerComponent::SetState(std::unique_ptr<PlayerState> newState)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);

    m_pCurrentState = std::move(newState);

    if (m_pCurrentState)
        m_pCurrentState->Enter(*this);
}

void PlayerComponent::MarkAsDead() 
{
   if (auto* playerSubj = GetOwner()->GetComponent<dae::SubjectComponent>())
   {
		playerSubj->Notify(dae::Event::PlayerDied, GetOwner());
   }

	SetState(std::make_unique<DeadState>());
}
bool PlayerComponent::IsInDeadState() const
{
    return dynamic_cast<DeadState*>(m_pCurrentState.get()) != nullptr;
}
