#include "CoilyComponent.h"

CoilyComponent::CoilyComponent(dae::GameObject& owner)
    : Component(owner),
    m_LastMoveDir{},
    m_IsPaused{}
{
    m_pOwner->GetComponent<dae::TextureComponent>()->SetTexture("Coily Spritesheet.png");
    m_pOwner->SetRenderLayer(RenderLayer::Characters);
}

CoilyComponent::~CoilyComponent()
{

}
bool CoilyComponent::GetPaused() const noexcept
{
    return m_IsPaused;
}
void CoilyComponent::SetPaused(bool paused) 
{
    m_IsPaused = paused;
}
void CoilyComponent::Update()
{
    if (m_IsPaused) return;

    // === collision ===
    if (dynamic_cast<ChasingState*>(m_pCurrentState.get()))
    {
        if (GetCoilyTile() == GetQbertTile())
        {
            GetOwner()->NotifyObservers(dae::Event::CoilyHitPlayer);
        }
    }

    constexpr float fixedDeltaTime = 1.f / 60.f;

    if (m_Jump.isJumping)
    {
        m_Jump.elapsed += fixedDeltaTime;

        float t = m_Jump.elapsed / m_Jump.duration;
        if (t >= 1.f)
        {
            m_pOwner->SetPosition(m_Jump.endPos.x, m_Jump.endPos.y);
            m_Jump.isJumping = false;
            m_Jump.waitTimer = 0.f;
            SetJumpSprite(m_LastMoveDir);
            return;
        }
        else
        {
            glm::vec2 pos = glm::mix(m_Jump.startPos, m_Jump.endPos, t);
            const float arcHeight = 10.f;
            pos.y -= sin(t * 3.14f * arcHeight);
            m_pOwner->SetPosition(pos.x, pos.y);
            return;
        }
    }

    if (m_Jump.waitTimer < m_Jump.waitDuration)
    {
        m_Jump.waitTimer += fixedDeltaTime;
        return;
    }

    if (m_pCurrentState)
    {
        m_pCurrentState->Update(*this);
    }
}


void CoilyComponent::Render() const
{
   
}

void CoilyComponent::SetState(std::unique_ptr<CoilyState> newState)
{
    if (m_pCurrentState)
        m_pCurrentState->Exit(*this);

    m_pCurrentState = std::move(newState);

    if (m_pCurrentState)
        m_pCurrentState->Enter(*this);
}
void CoilyComponent::TryMove(Direction direction)
{
    m_LastMoveDir = direction; 

    if (!m_CurrentTile || !m_pTileMap || m_Jump.isJumping || GameplayManagerComponent::GetInstance()->GetCurrentState() != GameState::Playing) return;

    auto [row, col] = m_CurrentTile->GetGridPosition();
    int newRow = row;
    int newCol = col;

    switch (direction)
    {
        case Direction::UpLeft:    newRow -= 1; newCol -= 1; break;
        case Direction::UpRight:   newRow -= 1; break;
        case Direction::DownLeft:  newRow += 1; break;
        case Direction::DownRight: newRow += 1; newCol += 1; break;
    }

    std::cout << "Coily trying to move to tile (" << newRow << ", " << newCol << ")\n";
    std::cout << "Jumping: " << m_Jump.isJumping << "\n";

    if (newRow < 0 || newRow >= static_cast<int>(m_pTileMap->size())) return;
    if (newCol < 0 || newCol >= static_cast<int>((*m_pTileMap)[newRow].size())) return;

    auto targetTile = (*m_pTileMap)[newRow][newCol];
    if (!targetTile) return;

    m_CurrentTile = targetTile;

    const glm::vec3 startPos = m_pOwner->GetTransform().GetPosition();
    const glm::vec3 endPos = targetTile->GetOwner()->GetTransform().GetPosition() + glm::vec3(m_xOffset, m_yOffset, 0.f);

    m_Jump.startPos = startPos;
    m_Jump.endPos = endPos;
    m_Jump.elapsed = 0.f;
    m_Jump.isJumping = true;
    SetJumpSprite(direction);
    QbertSoundLibrary::Play(SoundID::CoilySnakeJump);
}
glm::vec3 CoilyComponent::GetPosition() const
{
    return GetOwner()->GetWorldPosition();
}
void CoilyComponent::SetCurrentTile(std::shared_ptr<TileComponent> tile)
{
    m_CurrentTile = std::move(tile);

    if (m_CurrentTile)
    {
        const auto& pos = m_CurrentTile->GetOwner()->GetTransform().GetPosition();
        m_pOwner->SetPosition(pos.x + m_xOffset, pos.y + m_yOffset);
    }
}
const CoilyState* CoilyComponent::GetState() const noexcept
{
    return m_pCurrentState.get();
}
bool CoilyComponent::GetIsPlayer() const noexcept
{
    return m_IsPlayerControlled;
}
void CoilyComponent::SetPlayerControlled(bool isControlled)
{
    m_IsPlayerControlled = isControlled;
}
bool CoilyComponent::IsPlayerControlled() const noexcept
{
    return m_IsPlayerControlled;
}
void CoilyComponent::SetTileMap(const std::vector<std::vector<std::shared_ptr<TileComponent>>>& tileMap)
{
    m_pTileMap = &tileMap;
}
const QbertMoveComponent* CoilyComponent::GetQbert()
{
    return m_pQbert->GetComponent<QbertMoveComponent>();
}

std::shared_ptr<TileComponent> CoilyComponent::GetQbertTile()
{
    if (!m_pQbert) return nullptr;

    auto move = m_pQbert->GetComponent<QbertMoveComponent>();
    if (!move) return nullptr;

    return move->GetCurrentTile();
}
std::shared_ptr<TileComponent> CoilyComponent::GetCoilyTile()
{
    return m_CurrentTile;
}
void CoilyComponent::SetQbert(std::shared_ptr<dae::GameObject> qbert)
{ 
    m_pQbert = std::move(qbert); 
}

bool CoilyComponent::IsJumping() const
{
    return m_Jump.isJumping;
}

void CoilyComponent::SetJumpSprite(Direction dir)
{
    int frameIndex = 0;

    if (dynamic_cast<EggState*>(m_pCurrentState.get()))
    {
        frameIndex = m_Jump.isJumping ? 1 : 0;
    }
    else
    {
        switch (dir)
        {
        case Direction::UpRight:
            frameIndex = m_Jump.isJumping ? 3 : 2;
            break;
        case Direction::UpLeft:
            frameIndex = m_Jump.isJumping ? 5 : 4;
            break;
        case Direction::DownRight:
            frameIndex = m_Jump.isJumping ? 7 : 6;
            break;
        case Direction::DownLeft:
            frameIndex = m_Jump.isJumping ? 9 : 8;
            break;
        }
    }

    constexpr int frameWidth = 16;
    constexpr int frameHeight = 32;

    SDL_Rect src{ frameIndex * frameWidth, 0, frameWidth, frameHeight };
    if (auto* tex = m_pOwner->GetComponent<dae::TextureComponent>())
        tex->SetSourceRect(src);
}