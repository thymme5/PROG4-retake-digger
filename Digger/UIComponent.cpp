#include "UIComponent.h"
#include "TextComponent.h"
#include "ScoreManager.h"
#include "LevelManager.h"

UIComponent::UIComponent(dae::GameObject& owner)
    : dae::Component(owner)
{
    SetRenderLayer(RenderLayer::HUD);
    EnsureTextCached();

    if (m_pText)
        m_pText->SetRenderLayer(RenderLayer::HUD);
}

void UIComponent::EnsureTextCached()
{
    if (!m_pText)
        m_pText = GetOwner()->GetComponent<dae::TextComponent>();
}

void UIComponent::Update()
{
    EnsureTextCached();
    if (!m_pText) return;

    const int score = ScoreManager::GetInstance().Get();
    const int lives = ScoreManager::GetInstance().GetLives();
    int level = 1;
    if (LevelManager::IsAlive())
        level = LevelManager::GetInstance().GetCurrentLevelIndex();

    if (score != m_Score || lives != m_Lives || level != m_Level || m_Dirty)
    {
        m_Score = score;
        m_Lives = lives;
        m_Level = level;
        RefreshText();
        m_Dirty = false;
    }
   
}

void UIComponent::RefreshText()
{
    if (!m_pText) return;

    std::string line1 = "SCORE " + std::to_string(m_Score) + "  LIVES " + std::to_string(m_Lives);
    std::string line2 = "LEVEL " + std::to_string(m_Level);

    m_pText->SetText(line1 + " " + line2);
}
