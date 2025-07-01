#include "EggState.h"
#include "CoilyComponent.h"
#include "TransformingState.h"
#include "GameObject.h"

#include "TextureComponent.h"
void EggState::Enter(CoilyComponent& coily)
{
    std::cout << "Coily entered eggstate" << std::endl;
    coily;
}

void EggState::Update(CoilyComponent& coily)
{
    if (coily.IsJumping() || coily.GetPaused()) return;

    // alternate directions
    //TODO: get rid of the fucking enum and adapt this (i have no fucking clue how)
    if (m_LastDirection == Direction::DownLeft)
        m_LastDirection = Direction::DownRight;
    else
        m_LastDirection = Direction::DownLeft;

    coily.TryMove(m_LastDirection);

    auto [row, _] = coily.GetCoilyTile()->GetGridPosition();
    
    QbertSoundLibrary::Play(SoundID::CoilyEggJump);

    if (row >= 6)
    {
        coily.SetState(std::make_unique<TransformingState>());
    }
}

void EggState::Exit(CoilyComponent&)
{

}