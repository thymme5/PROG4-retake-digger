#pragma once
#include "PlayerState.h"

class AliveState final : public PlayerState
{
public:
    void Enter(PlayerComponent& player) override;
    void Update(PlayerComponent& player) override;
    void Exit(PlayerComponent& player) override;
};
