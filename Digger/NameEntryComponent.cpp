#include "NameEntryComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "ScoreManager.h"
#include "HighScoreManager.h"
#include "SceneManager.h"
#include "GameModeManager.h"
#include "HighScoreMenu.h"
#include "MainMenu.h"

NameEntryComponent::NameEntryComponent(dae::GameObject& owner)
    : Component(owner)
{
    SetRenderLayer(RenderLayer::HUD);

    const auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 48);
    m_Text = owner.AddComponent<dae::TextComponent>(owner, "", font);

    RefreshDisplay();

    const auto windowWidth = dae::Renderer::GetInstance().GetWindowSize().x;
    const float x = (windowWidth - static_cast<float>(m_Text->GetTextureSize().x)) / 2.f;
    owner.SetLocalPosition(x, 260.f);
}

void NameEntryComponent::ChangeLetter(int direction)
{
    char& letter = m_Initials[m_CursorIndex];
    int index = (letter - 'A' + direction + 26) % 26;
    letter = static_cast<char>('A' + index);

    RefreshDisplay();
}

void NameEntryComponent::MoveCursor(int direction)
{
    m_CursorIndex = (m_CursorIndex + direction + 3) % 3;
    RefreshDisplay();
}

void NameEntryComponent::Confirm()
{
    std::string initials(m_Initials.begin(), m_Initials.end());
    int score = ScoreManager::GetInstance().Get();

    HighscoreManager::GetInstance().AddHighscore(initials, score);
    HighscoreManager::GetInstance().SaveHighscores();
    ScoreManager::GetInstance().ResetForNewGame();

    GameModeManager::GetInstance().SetMode(std::make_unique<MainMenu>());
}

void NameEntryComponent::RefreshDisplay()
{
    std::string display;
    for (int i = 0; i < 3; ++i)
    {
        if (i == m_CursorIndex)
            display += '[' + std::string(1, m_Initials[i]) + ']';
        else
            display += ' ' + std::string(1, m_Initials[i]) + ' ';
    }

    m_Text->SetText(display);
    m_Text->Update();
}

void NameEntryComponent::Update()
{

}
