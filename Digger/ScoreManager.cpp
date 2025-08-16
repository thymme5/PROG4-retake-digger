#include <iostream>

#include "ScoreManager.h"
#include "SubjectComponent.h"
#include "LevelManager.h"
void ScoreManager::OnNotify(dae::Event event, dae::GameObject* obj)
{
	std::cout << "[ScoreManager] Received event: " << static_cast<int>(event) << std::endl;

    switch (event)
    {
    case dae::Event::EmeraldCollected:          AddEmerald();            break;
    case dae::Event::GoldCollected:             AddGoldNugget();         break;
    case dae::Event::PlayerCollected8Emeralds:  AddEightEmeraldStreakBonus(); break;

    case dae::Event::EnemyKilled:
        AddNobbin();
        break;
    case dae::Event::PlayerDied:
        LoseLife(1);
        break;
    default:
        break;
    }
}
void ScoreManager::LoseLife(int n) 
{ 
    m_lives = (m_lives > n) ? (m_lives - n) : 0;

    if (m_lives == 0)
    {
        // Finis game if player has no lives (obviously)
        LevelManager::GetInstance().FinishGame();
    }
}
void ScoreManager::Subscribe(dae::SubjectComponent& subject)
{
    subject.AddObserver(this);
}

void ScoreManager::Unsubscribe(dae::SubjectComponent& subject)
{
    subject.RemoveObserver(this);
}
