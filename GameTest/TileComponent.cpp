#include "TileComponent.h"

#include "GameObject.h"
#include "TextureComponent.h"

#include "ColorMap.h"

extern std::unordered_map<std::string, SDL_Rect> g_ColorSpriteMap;

TileComponent::TileComponent(dae::GameObject& owner)
	: Component(owner)
{
}

void TileComponent::Update() 
{
	if (m_IsFlashing)
	{
		m_FlashTimer += 1.0f / 60.0f;

		if (m_FlashTimer >= m_FlashInterval)
		{
			m_FlashTimer = 0.0f;
			m_FlashVisibleState = !m_FlashVisibleState;

			if (auto* texture = m_pOwner->GetComponent<dae::TextureComponent>())
			{
				const std::string colorToSet = m_FlashVisibleState ? m_TargetColor : m_StartColor;

				if (g_ColorSpriteMap.contains(colorToSet))
				{
					const SDL_Rect& srcRect = g_ColorSpriteMap.at(colorToSet);
					texture->SetSourceRect(srcRect);
				}
			}
		}
	}
	else
	{
		auto textureComp = m_pOwner->GetComponent<dae::TextureComponent>();
		if (!textureComp) return;

		const std::string color = GetCurrentColor();

		auto it = g_ColorSpriteMap.find(color);
		if (it != g_ColorSpriteMap.end())
		{
			textureComp->SetSourceRect(it->second);
		}
	}
}

void TileComponent::Render() const {}

void TileComponent::SetID(int id)
{
	m_ID = id;
}

int TileComponent::GetID() const
{
	return m_ID;
}

void TileComponent::SetState(TileState state)
{
	m_CurrentState = state;
}

TileState TileComponent::GetState() const
{
	return m_CurrentState;
}

void TileComponent::SetTargetState(TileState targetState)
{
	m_TargetState = targetState;
}

bool TileComponent::IsCompleted() const
{
	return m_IsCompleted;
}

void TileComponent::SetColorStates(const std::string& startColor, const std::string& intermediateColor, const std::string& targetColor)
{
	m_StartColor = startColor;
	m_IntermediateColor = intermediateColor;
	m_TargetColor = targetColor;
}

std::string TileComponent::GetCurrentColor() const
{
	switch (m_CurrentState)
	{
	case TileState::Default:
		return m_StartColor;
	case TileState::Intermediate:
		return m_IntermediateColor;
	case TileState::Target:
		return m_TargetColor;
	default:
		return {};
	}
}

void TileComponent::OnStepped(dae::GameObject* actor)
{
	switch (m_TileRule)
	{
	case TileRule::OneStepToTarget:
		if (m_CurrentState != TileState::Target)
		{
			m_CurrentState = TileState::Target;
			m_IsCompleted = true;
		}
		break;

	case TileRule::TwoStepsToTarget:
		if (m_CurrentState == TileState::Default && !m_IntermediateColor.empty())
		{
			m_CurrentState = TileState::Intermediate;
		}
		else if (m_CurrentState == TileState::Intermediate)
		{
			m_CurrentState = TileState::Target;
			m_IsCompleted = true;
		}
		break;

	case TileRule::ToggleColor:
		if (m_CurrentState == TileState::Target)
		{
			m_CurrentState = TileState::Default;
			m_IsCompleted = false;
		}
		else
		{
			m_CurrentState = TileState::Target;
			m_IsCompleted = true;
		}
		break;
	}

	actor->NotifyObservers(dae::Event::TileStateChanged);

	if (auto* texture = GetOwner()->GetComponent<dae::TextureComponent>())
	{
		std::string colorToSet = GetCurrentColor();

		if (g_ColorSpriteMap.contains(colorToSet))
		{
			const SDL_Rect& srcRect = g_ColorSpriteMap.at(colorToSet);
			texture->SetSourceRect(srcRect);
		}
	}
}
void TileComponent::SetTileRule(TileRule rule)
{
	m_TileRule = rule;
}

void TileComponent::SetGridPosition(int row, int col)
{
	m_Row = row;
	m_Col = col;
}

std::pair<int, int> TileComponent::GetGridPosition() const
{
	return { m_Row, m_Col };
}

void TileComponent::StartFlashing()
{
	m_IsFlashing = true;
	m_FlashTimer = 0.0f;
	m_FlashVisibleState = true;
}

void TileComponent::StopFlashing()
{
	m_IsFlashing = false;

	// Set final state color
	auto* texture = m_pOwner->GetComponent<dae::TextureComponent>();
	if (texture && g_ColorSpriteMap.contains(m_TargetColor))
	{
		texture->SetSourceRect(g_ColorSpriteMap.at(m_TargetColor));
	}
}
