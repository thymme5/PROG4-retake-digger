#pragma once
#include "Command.h"
#include "LevelManager.h"
class FinishRoundCommand final : public dae::Command
{
public:
    FinishRoundCommand() = default;
  

    void Execute() override
    {
        LevelManager::GetInstance().LoadNextLevel();
    }
};
