#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include "MainMenu.h"
#include "Subject.h"

class Subject;

class LevelManager final : public dae::Singleton<LevelManager>, public dae::Subject
{
public:
    LevelManager() = default;
    ~LevelManager() override = default;

    void Initialize();
    void LoadLevel(int index);
    void LoadNextLevel();

    int GetCurrentLevelIndex() const { return m_CurrentLevelIndex; };

private:
    int m_CurrentLevelIndex = 1;
    int m_TotalLevels = 3;

    void HandleLevelCompleted(dae::GameObject* player);
    void ResetLevelState();
};
