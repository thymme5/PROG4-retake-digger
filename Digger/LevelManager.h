#pragma once
#include "Singleton.h"
#include "Observer.h"
#include "GameObject.h"

class LevelManager final : public dae::Observer, public dae::Singleton<LevelManager>
{
public:
    LevelManager() = default;
    ~LevelManager() override = default;

    void Initialize();
    void LoadLevel(int index);
    void LoadNextLevel();

    void OnNotify(dae::Event event, dae::GameObject* source) override;

private:
    int m_CurrentLevelIndex = 1;
    int m_TotalLevels = 3;

    void HandleLevelCompleted(dae::GameObject* player);
    void ResetLevelState();
};
