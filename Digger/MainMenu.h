#pragma once
#include "GameMode.h"
#include <iostream>

class MainMenu : public GameMode
{
public:
    void Enter() override;
    void Exit() override;
    void Update() override;
private:
    std::string m_SceneName{ "MainMenuScene" };
};
