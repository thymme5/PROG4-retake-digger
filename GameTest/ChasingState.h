#pragma once
#include "CoilyState.h"

class ChasingState final : public CoilyState
{
public:
    void Enter(CoilyComponent& coily) override;
    void Update(CoilyComponent& coily) override;
    void Exit(CoilyComponent& coily) override;
};
