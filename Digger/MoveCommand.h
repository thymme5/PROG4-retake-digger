#pragma once
#include "Command.h"
#include "PlayerComponent.h"

class MoveCommand final : public dae::Command
{
public:
    MoveCommand(PlayerComponent& player, int dRow, int dCol)
        : m_pPlayer{ &player }, m_DeltaRow{ dRow }, m_DeltaCol{ dCol }
    {
    }

    void Execute() override
    {
        if (m_pPlayer)
            m_pPlayer->Move(m_DeltaRow, m_DeltaCol);
    }

private:
    PlayerComponent* m_pPlayer{};
    int m_DeltaRow{};
    int m_DeltaCol{};
};
