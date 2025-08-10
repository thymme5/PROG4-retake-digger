
// UIComponent.cpp
#include "UIComponent.h"
#include "SubjectComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include <cstdio> // std::snprintf

void UIComponent::EnsureTextCached()
{
    if (!m_pText)
        m_pText = GetOwner()->GetComponent<dae::TextComponent>();
}

void UIComponent::Update()
{
    EnsureTextCached();
    if (m_Dirty)
    {
        RefreshText();
        m_Dirty = false;
    }
}

void UIComponent::RefreshText()
{
    if (!m_pText) return;

    std::string line1 = std::string("SCORE ") + std::to_string(m_Score) + "  LIVES " + std::to_string(m_Lives);
    std::string line2 = "LEVEL " + std::to_string(m_Level);

    m_pText->SetText(line1 + " " + line2);
}

void UIComponent::Observe(dae::SubjectComponent& subject)
{
    subject.AddObserver(this);
    m_Sources.push_back(&subject);
}

void UIComponent::StopObservingAll()
{
   
}
 
void UIComponent::OnNotify(dae::Event event, dae::GameObject*)
{
    switch (event)
    {
    case dae::Event::EmeraldCollected:  m_Score += 25; std::cout << "Emerald collected: +25 points \n";  break;
    case dae::Event::GoldCollected:     m_Score += 500; std::cout << "Gold collected: +500 points \n"; break;
    case dae::Event::PlayerDied:        --m_Lives; if (m_Lives < 0) m_Lives = 0; break;
    case dae::Event::LevelCompleted:    ++m_Level; break;
    default: break;
    }

    // Extra life every 20,000 points
    while (m_Score >= m_NextLifeScore)
    {
        ++m_Lives;
        m_NextLifeScore += 20000;
    }

    m_Dirty = true;
}
