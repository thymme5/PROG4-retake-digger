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
#include "GameModeManager.h"
#include "SinglePlayerMode.h"
void AliveState::Enter(PlayerComponent& player)
{
    auto& input = dae::InputManager::GetInstance();
    int playerID = player.GetPlayerID();

    const bool isSolo = dynamic_cast<SinglePlayerMode*>(GameModeManager::GetInstance().GetCurrentGameMode()) != nullptr;

    if (playerID == 0)
    {
        std::cout << "[AliveState] Player 1 controls initialized.\n";

        // Keyboard movement (held)
        input.BindCommand(SDLK_UP, KeyState::Held, std::make_shared<MoveCommand>(player, -1, 0));
        input.BindCommand(SDLK_DOWN, KeyState::Held, std::make_shared<MoveCommand>(player, 1, 0));
        input.BindCommand(SDLK_LEFT, KeyState::Held, std::make_shared<MoveCommand>(player, 0, -1));
        input.BindCommand(SDLK_RIGHT, KeyState::Held, std::make_shared<MoveCommand>(player, 0, 1));

        // Keyboard fireball (just pressed)
        input.BindCommand(SDLK_SPACE, KeyState::Pressed, std::make_shared<FireballCommand>(player));

        if (isSolo)
        {
            std::cout << "[AliveState] Solo mode - enabling controller input for Player 1.\n";

            // Controller movement (held)
            input.BindCommand(0, GamepadButton::DPadUp, KeyState::Held, std::make_shared<MoveCommand>(player, -1, 0));
            input.BindCommand(0, GamepadButton::DPadDown, KeyState::Held, std::make_shared<MoveCommand>(player, 1, 0));
            input.BindCommand(0, GamepadButton::DPadLeft, KeyState::Held, std::make_shared<MoveCommand>(player, 0, -1));
            input.BindCommand(0, GamepadButton::DPadRight, KeyState::Held, std::make_shared<MoveCommand>(player, 0, 1));

            // Controller fireball (just pressed)
            input.BindCommand(0, GamepadButton::A, KeyState::Pressed, std::make_shared<FireballCommand>(player));
        }
    }
    else if (playerID == 1)
    {
        std::cout << "[AliveState] Player 2 controls initialized.\n";

        // Controller movement (held)
        input.BindCommand(1, GamepadButton::DPadUp, KeyState::Held, std::make_shared<MoveCommand>(player, -1, 0));
        input.BindCommand(1, GamepadButton::DPadDown, KeyState::Held, std::make_shared<MoveCommand>(player, 1, 0));
        input.BindCommand(1, GamepadButton::DPadLeft, KeyState::Held, std::make_shared<MoveCommand>(player, 0, -1));
        input.BindCommand(1, GamepadButton::DPadRight, KeyState::Held, std::make_shared<MoveCommand>(player, 0, 1));

        // Controller fireball (just pressed)
        input.BindCommand(1, GamepadButton::A, KeyState::Pressed, std::make_shared<FireballCommand>(player));
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
