#pragma once
#include "GameMode.h"
#include <iostream>

class VersusMode : public GameMode
{
public:
    VersusMode(int levelIndex);
    void Enter() override;
    void Exit() override;
    void Update() override;
private:
    std::string m_SceneName{ "VersusScene" };
    int m_LevelIndex;
};
