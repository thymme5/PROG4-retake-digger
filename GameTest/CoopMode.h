#pragma once
#include "GameMode.h"
#include <iostream>

class CoopMode : public GameMode
{
public:

    CoopMode(int levelIndex);
    void Enter() override;
    void Exit() override;
    void Update() override;
private:
    std::string m_SceneName{"CoopScene"};
    int m_LevelIndex;
};
