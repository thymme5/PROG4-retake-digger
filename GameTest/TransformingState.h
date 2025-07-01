#pragma once
#include "CoilyState.h"
#include <memory>

class TransformingState final : public CoilyState
{
public:
    void Enter(CoilyComponent& coily) override;
    void Update(CoilyComponent& coily) override;
    void Exit(CoilyComponent& coily) override;

private:
    float m_AccumulatedTime = 0.0f;
};
