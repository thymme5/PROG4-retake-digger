#pragma once

class CoilyComponent;

class CoilyState {
public:
    virtual ~CoilyState() = default;

    virtual void Enter(CoilyComponent& coily) = 0;
    virtual void Update(CoilyComponent& coily) = 0;
    virtual void Exit(CoilyComponent& coily) = 0;
};
