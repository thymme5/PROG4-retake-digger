#pragma once

class PlayerComponent;

class PlayerState
{
public:
    virtual ~PlayerState() = default;

    virtual void Enter(PlayerComponent& player) = 0;
    virtual void Update(PlayerComponent& player) = 0;
    virtual void Exit(PlayerComponent& player) = 0;
};
