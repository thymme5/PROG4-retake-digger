#pragma once
#include "Command.h"
#include "MainMenuUIComponent.h"
#include "GameModeManager.h"
#include "SinglePlayerMode.h"
#include "CoopMode.h"
#include "VersusMode.h"
#include "HighScoreMenu.h"
class EnterGameMode final : public dae::Command
{
public:
    explicit EnterGameMode(MainMenuUIComponent* menuUI)
        : m_pMenuUI(menuUI), m_UseIndexFromMenu(true)
    {
    }

    explicit EnterGameMode(int index)
        : m_Selected(index), m_UseIndexFromMenu(false)
    {
    }

    void Execute() override
    {
        int selected = m_UseIndexFromMenu && m_pMenuUI
            ? m_pMenuUI->GetSelectedIndex()
            : m_Selected;

        switch (selected)
        {
        case 0:
            GameModeManager::GetInstance().SetMode(std::make_unique<SinglePlayerMode>(1));
            break;
        case 1:
            GameModeManager::GetInstance().SetMode(std::make_unique<CoopMode>(1));
            break;
        case 2:
            GameModeManager::GetInstance().SetMode(std::make_unique<VersusMode>(1));
            break;
        case 3:
            GameModeManager::GetInstance().SetMode(std::make_unique<HighScoreMenu>());
            break;
        case 4:
            GameModeManager::GetInstance().SetMode(std::make_unique<MainMenu>());
            break;
        default:
            break;
        }
    }

private:
    MainMenuUIComponent* m_pMenuUI{};
    int m_Selected{};
    bool m_UseIndexFromMenu{ true };
};
