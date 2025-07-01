#pragma once

class GameMode
{
public:
    virtual ~GameMode() = default;

    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Update() = 0;
};
