#include "TransformingState.h"
#include "CoilyComponent.h"
#include "TextureComponent.h"

#include "ChasingState.h"
#include <memory>

void TransformingState::Enter(CoilyComponent& coily)
{
    coily;
}

void TransformingState::Update(CoilyComponent& coily)
{
    m_AccumulatedTime += 1.0f / 60.0f; //Fixed timesteps

    if (m_AccumulatedTime >= 1.0f) //After 1 second
    {
        coily.SetState(std::make_unique<ChasingState>());
        coily;
    }
}

void TransformingState::Exit(CoilyComponent&)
{
    
}
