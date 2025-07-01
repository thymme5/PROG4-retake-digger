#pragma once
#include "GameMode.h"
#include <iostream>

class VictoryScreen : public GameMode
{
public:
    void Enter() override;
    void Exit() override;
    void Update() override;
private:
    std::string m_SceneName{ "VictoryScreen" };
};
