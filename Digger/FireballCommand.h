#pragma once
#include "Command.h"
#include "PlayerComponent.h"

class FireballCommand final : public dae::Command
{
public:
    explicit FireballCommand(PlayerComponent& player)
        : m_Player(player)
    {
    }

    void Execute() override
    {
        m_Player.ShootFireball();
    }

private:
    PlayerComponent& m_Player;
};
