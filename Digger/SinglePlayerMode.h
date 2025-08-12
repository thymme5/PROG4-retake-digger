#pragma once
#include "GameMode.h"
#include <iostream>

class SinglePlayerMode : public GameMode
{
public:
    SinglePlayerMode(int levelIndex);

    void Enter() override;
    void Exit() override;
    void Update() override;
private:
    std::string m_SceneName{ "SinglePlayerModeScene" };
    int m_LevelIndex;
};
