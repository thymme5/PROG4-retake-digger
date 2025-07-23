#include "AliveState.h"
#include "PlayerComponent.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "SubjectComponent.h"
#include <SDL.h>
#include "TextureComponent.h"
#include "InputManager.h"
#include "MoveCommand.h"

void AliveState::Enter(PlayerComponent& player)
{
    auto& input = dae::InputManager::GetInstance();

    std::cout << "entering alive state " << std::endl;

    // WASD or arrow keys, for example
    input.BindCommand(SDLK_w, KeyState::Down, std::make_shared<MoveCommand>(player, -1, 0));
    input.BindCommand(SDLK_s, KeyState::Down, std::make_shared<MoveCommand>(player, 1, 0));
    input.BindCommand(SDLK_a, KeyState::Down, std::make_shared<MoveCommand>(player, 0, -1));
    input.BindCommand(SDLK_d, KeyState::Down, std::make_shared<MoveCommand>(player, 0, 1));
}


void AliveState::Update(PlayerComponent& player)
{
	const int col = player.GetOwner()->GetComponent<PlayerComponent>()->GetCol();
	const int row = player.GetOwner()->GetComponent<PlayerComponent>()->GetRow();

    auto tile = TileManager::GetInstance().GetTile(row, col);
    if (tile && !tile->IsDug())
    {
        tile->SetDug(true);

        if (auto tex = tile->GetOwner()->GetComponent<dae::TextureComponent>())
        {

        }

        if (auto subject = player.GetOwner()->GetComponent<dae::SubjectComponent>())
        {
            //subject->Notify(dae::Event::TileStateChanged, tile->GetOwner());
        }
    }

    // TODO: Check if player collected emerald or collided with gold bag
}

void AliveState::Exit(PlayerComponent& /*player*/)
{
}
