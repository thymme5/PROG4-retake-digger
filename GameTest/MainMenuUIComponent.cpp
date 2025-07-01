#include "MainMenuUIComponent.h"
#include "QbertSoundLibrary.h"
MainMenuUIComponent::MainMenuUIComponent(dae::GameObject& owner) :
	dae::Component(owner), m_ArrowBaseY{}, m_ArrowX{}
{
	m_LogoTexture = dae::ResourceManager::GetInstance().LoadTexture("Game Title.png");
}
void MainMenuUIComponent::MoveArrow(float directionSign)
{
	if (directionSign == 0) return;

	constexpr int totalOptions = 4;
	constexpr float spacing = 50.f;

	if (directionSign > 0)
		m_SelectedIndex = (m_SelectedIndex + 1) % totalOptions;
	else
		m_SelectedIndex = (m_SelectedIndex + totalOptions - 1) % totalOptions;

	if (const auto arrow = m_pArrow.lock())
	{
		float newY = m_ArrowBaseY + m_SelectedIndex * spacing;
		arrow->SetPosition(m_ArrowX, newY);
	}
	QbertSoundLibrary::Play(SoundID::ChangeSelection);
}

void MainMenuUIComponent::Update() 
{

}
void MainMenuUIComponent::Render() const 
{
	// get logo size and center it on screen
	glm::ivec2 logoSize = m_LogoTexture->GetSize();
	float logoX = (dae::Renderer::GetInstance().GetWindowSize().x - logoSize.x) / 2.0f;
	float logoY = 100.0f;

	dae::Renderer::GetInstance().RenderTexture(*m_LogoTexture, logoX, logoY);
}

void MainMenuUIComponent::SetArrow(const std::shared_ptr<dae::GameObject>& arrow)
{ 
	m_pArrow = arrow; 
	m_ArrowBaseY = arrow->GetTransform().GetPosition().y;
	m_ArrowX = arrow->GetTransform().GetPosition().x;
}
