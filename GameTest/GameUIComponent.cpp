#include <iostream>

#include "GameUIComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "QbertSoundLibrary.h"

using namespace dae;

GameUIComponent::GameUIComponent(GameObject& owner)
    : Component(owner), m_Lives(3), m_Score(0), m_IsDirty{true}
{
    m_HeartTexture = dae::ResourceManager::GetInstance().LoadTexture("heart.png");
}

void GameUIComponent::OnNotify(Event event, GameObject* pGameObject)
{
    //temp value to stop compiler from bitching
    GameObject* gameObject = pGameObject;
    gameObject;

    switch (event)
    {
    case dae::Event::PlayerDied:
        break;
    case dae::Event::PlayerLandedOnTile:
        break;
    case dae::Event::TileStateChanged:
        m_Score += 25;
        m_IsDirty = true;
        break;
    case dae::Event::roundComplete:
        m_Round++;
        m_IsDirty = true;
        break;
    case dae::Event::LevelComplete:
        m_Round = 1;
        m_Level++;
        m_IsDirty = true;
        break;
    case dae::Event::CoilyHitPlayer:
        break;
    default:
        break;
    }
}
int GameUIComponent::GetLives() const noexcept
{
    return m_Lives;
}
void GameUIComponent::UpdateLives()
{
    if (m_Lives > 0)
    {
        m_Lives--;
        m_IsDirty = true;
    }
    else
    {
        
    }
}
int GameUIComponent::GetScore() const noexcept
{
    return m_Score;
}
void GameUIComponent::UpdateScore(int newScore)
{
    if (m_Score != newScore)
    {
        m_Score = newScore;
    }
}

void GameUIComponent::Update() 
{
    if (m_IsDirty)
    {
        std::string text{ "SCORE: " + std::to_string(m_Score) };
        m_pOwner->GetComponent<TextComponent>()->SetText(text);

        if (m_pRoundText)
            m_pRoundText->SetText("LEVEL: " + std::to_string(m_Round));
        m_IsDirty = false;
    }
}

void GameUIComponent::Render() const
{
    if (!m_HeartTexture) return;

    const float spacing = 32.0f;
    const float baseX = 10.0f;
    const float baseY = 100.0f;

    for (int i = 0; i < m_Lives; ++i)
    {
        float y = baseY + i * spacing;
        dae::Renderer::GetInstance().RenderTexture(*m_HeartTexture, baseX, y);
    }
}
