#pragma once
#include "Command.h"
#include "GameplayManagerComponent.h"

class FinishRoundCommand final : public dae::Command
{
public:
    FinishRoundCommand(GameplayManagerComponent* manager)
        : m_pManager(manager) {
    }

    void Execute() override
    {
        if (m_pManager)
        {
            m_pManager->ForceCompleteRound();
        }
    }

private:
    GameplayManagerComponent* m_pManager{};
};
