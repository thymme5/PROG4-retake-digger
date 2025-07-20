#pragma once

// While the implementation of GameMode isn't a perfect example of the state machine pattern, I enjoy it's simplicity and chose to re-use it.

class GameMode
{
public:
    virtual ~GameMode() = default;

    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Update() = 0;
};
