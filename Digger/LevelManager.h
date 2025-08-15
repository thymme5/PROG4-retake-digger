#pragma once
#include "Singleton.h"
#include "GameObject.h"
#include "MainMenu.h"
#include "Subject.h"

class LevelManager final : public dae::Singleton<LevelManager>, public dae::Subject
{
public:
    LevelManager();
    ~LevelManager() override;

    void Initialize();
    void LoadLevel(int index);
    void LoadNextLevel();

    void QueueNextLevel();
    void ProcessQueuedLoads();
    bool IsLevelQueued() const { return m_queuedNext; }

    int GetCurrentLevelIndex() const { return m_CurrentLevelIndex; }
    static bool IsAlive();

private:
    int m_CurrentLevelIndex = 1;
    int m_TotalLevels = 3;
    bool m_queuedNext = false;

    void HandleLevelCompleted(dae::GameObject* player);
    void ResetLevelState();
};