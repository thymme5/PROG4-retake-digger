#include "QbertMoveComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "LevelBuilder.h"
#include "TextureComponent.h"
#include "QbertSoundLibrary.h"
#include "GameplayManagerComponent.h"

QbertMoveComponent::QbertMoveComponent(dae::GameObject& pOwner)
	: dae::Component(pOwner)
{
	//set rendering layer to characters
	m_pOwner->SetRenderLayer(RenderLayer::Characters);
}

void QbertMoveComponent::Update()
{
	if (!m_Jump.isJumping) return;

	constexpr float fixedDeltaTime = 1.0f / 60.0f; //standard 60FPS
	m_Jump.elapsed += fixedDeltaTime;

	float t = m_Jump.elapsed / m_Jump.duration;
	if (t >= 1.f)
	{
		m_pOwner->SetPosition(m_Jump.endPos.x, m_Jump.endPos.y);
		m_Jump.isJumping = false;

		//play sound and let tile know that it got stepped on
		QbertSoundLibrary::Play(SoundID::QbertJump);

		if (m_CurrentTile)
			m_CurrentTile->OnStepped(m_pOwner);
		return;
	}

	//crazy linear interpolation
	glm::vec2 pos = glm::mix(m_Jump.startPos, m_Jump.endPos, t);

	//math go brr brr (patapim)
	const float arcHeight = 10.f;
	pos.y -= sin(t * 3.14f * arcHeight);
		
	m_pOwner->SetPosition(pos.x, pos.y);
}


void QbertMoveComponent::Render() const
{

}

void QbertMoveComponent::SetCurrentTile(std::shared_ptr<TileComponent> tile)
{
	m_CurrentTile = std::move(tile);

	if (m_CurrentTile)
	{
		const auto& pos = m_CurrentTile->GetOwner()->GetTransform().GetPosition();
		m_pOwner->SetPosition(pos.x + m_xOffset, pos.y + m_yOffset);
	}
}

void QbertMoveComponent::SetTileMap(const std::vector<std::vector<std::shared_ptr<TileComponent>>>& tileMap)
{
	m_pTileMap = &tileMap;
}
std::shared_ptr<TileComponent> QbertMoveComponent::GetCurrentTile()
{
	return m_CurrentTile;
}
void QbertMoveComponent::TryMove(Direction direction)
{
	//reject movement if we don't have tile information or if the bitch is already jumping
	if (!m_CurrentTile || !m_pTileMap || m_Jump.isJumping || GameplayManagerComponent::GetInstance()->GetCurrentState() != GameState::Playing) return;

	auto textureComp = m_pOwner->GetComponent<dae::TextureComponent>();
	if (!textureComp) return;

	constexpr int frameWidth = 17;
	constexpr int frameHeight = 16;

	auto [row, col] = m_CurrentTile->GetGridPosition();
	int newRow = row;
	int newCol = col;

	int frameIndex = 0; 

	switch (direction)
	{
	case Direction::UpLeft:
		newRow -= 1;
		newCol -= 1;
		frameIndex = 1;
		break;
	case Direction::UpRight:
		newRow -= 1;
		frameIndex = 0;
		break;
	case Direction::DownLeft:
		newRow += 1;
		frameIndex = 3;
		break;
	case Direction::DownRight:
		newRow += 1;
		newCol += 1;
		frameIndex = 2;
		break;
	}

	SDL_Rect src{ frameIndex * frameWidth, 0, frameWidth, frameHeight };
	textureComp->SetSourceRect(src);

	//OUT OF BOUNDS IN THIS CONTEXT MEANS QBERT WILL DIE.
	//check row bounds
	if (newRow < 0 || newRow >= static_cast<int>(m_pTileMap->size()))
	{
		m_pOwner->NotifyObservers(dae::Event::CoilyHitPlayer);
		return;
	}

	//check col bounds
	if (newCol < 0 || newCol >= static_cast<int>((*m_pTileMap)[newRow].size()))
	{
		m_pOwner->NotifyObservers(dae::Event::CoilyHitPlayer);
		return;
	}

	auto targetTile = (*m_pTileMap)[newRow][newCol];
	if (!targetTile)
	{
		std::cout << "Null tile at position\n";
		return;
	}

	m_CurrentTile = targetTile;

	//old lame teleport 
	//const auto& pos = m_CurrentTile->GetOwner()->GetTransform().GetPosition();
	//m_pOwner->SetPosition(pos.x + m_xOffset, pos.y + m_yOffset);

	//new exciting jump "animation"
	const glm::vec3 startPos = m_pOwner->GetTransform().GetPosition();
	const glm::vec3 endPos = targetTile->GetOwner()->GetTransform().GetPosition() + glm::vec3(m_xOffset, m_yOffset, 0.f);

	m_Jump.startPos = startPos;
	m_Jump.endPos = endPos;
	m_Jump.elapsed = 0.f;
	m_Jump.duration = 0.4f;
	m_Jump.isJumping = true;
}
