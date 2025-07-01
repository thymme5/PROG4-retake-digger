#pragma once
#include "Command.h"
#include "GameModeManager.h"
#include "MainMenu.h"

class BackToMenuCommand final : public dae::Command
{
public:
    BackToMenuCommand() :
        dae::Command()
    {
    };

    void Execute() override
    {
        GameModeManager::GetInstance().SetMode(std::make_unique<MainMenu>());   
    }
};
