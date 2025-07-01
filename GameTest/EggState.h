#pragma once
#include "CoilyState.h"
#include "CoilyComponent.h"

class EggState final : public CoilyState {
public:
    void Enter(CoilyComponent& coily) override;
    void Update(CoilyComponent& coily) override;
    void Exit(CoilyComponent& coily) override;

private:
    Direction m_LastDirection{};
};
