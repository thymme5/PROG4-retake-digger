#include "AliveState.h"
#include "PlayerComponent.h"
#include "TileManager.h"
#include "TileComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include "SubjectComponent.h"
#include <SDL.h>
#include "TextureComponent.h"
#include "MoveCommand.h"
#include "FireballCommand.h"

void AliveState::Enter(PlayerComponent& player)
{
    auto& input = dae::InputManager::GetInstance();
    int playerID = player.GetPlayerID();

    if (playerID == 0)
    {
		std::cout << "[AliveState] Player 1 controls initialized.\n";

        // Keyboard controls for Player 1
        input.BindCommand(SDLK_UP, KeyState::Down, std::make_shared<MoveCommand>(player, -1, 0));
        input.BindCommand(SDLK_DOWN, KeyState::Down, std::make_shared<MoveCommand>(player, 1, 0));
        input.BindCommand(SDLK_LEFT, KeyState::Down, std::make_shared<MoveCommand>(player, 0, -1));
        input.BindCommand(SDLK_RIGHT, KeyState::Down, std::make_shared<MoveCommand>(player, 0, 1));
        input.BindCommand(SDLK_SPACE, KeyState::Down, std::make_shared<FireballCommand>(player));
    }
    else if (playerID == 1)
    {
        std::cout << "[AliveState] Player 2 controls initialized.\n";

        // Controller controls for Player 2
        input.BindCommand(0, GamepadButton::DPadUp, KeyState::Down, std::make_shared<MoveCommand>(player, -1, 0));
        input.BindCommand(0, GamepadButton::DPadDown, KeyState::Down, std::make_shared<MoveCommand>(player, 1, 0));
        input.BindCommand(0, GamepadButton::DPadLeft, KeyState::Down, std::make_shared<MoveCommand>(player, 0, -1));
        input.BindCommand(0, GamepadButton::DPadRight, KeyState::Down, std::make_shared<MoveCommand>(player, 0, 1));
        input.BindCommand(0, GamepadButton::A, KeyState::Down, std::make_shared<FireballCommand>(player));
    }
}


void AliveState::Update(PlayerComponent& player)
{
	const int col = player.GetOwner()->GetComponent<PlayerComponent>()->GetCol();
	const int row = player.GetOwner()->GetComponent<PlayerComponent>()->GetRow();

    auto tile = TileManager::GetInstance().GetTile(row, col);
    if (tile && !tile->IsDug())
    {
        tile->SetDug(true);
        
        if (auto subject = player.GetOwner()->GetComponent<dae::SubjectComponent>())
        {
            subject->Notify(dae::Event::TileDug, tile->GetOwner());
        }
    }
}

void AliveState::Exit(PlayerComponent& /*player*/)
{
}
