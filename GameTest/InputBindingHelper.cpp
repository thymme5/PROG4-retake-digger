#include "InputBindingHelper.h"
#include "InputManager.h"
#include "MoveCommand.h"
#include "gamepad.h"
#include <SDL.h>

using D = Direction;

void InputBindingHelper::BindPlayer1KeyboardInputs(dae::GameObject* qbert)
{
    auto& input = dae::InputManager::GetInstance();

    input.BindCommand(SDLK_UP, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::UpLeft));
    input.BindCommand(SDLK_RIGHT, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::UpRight));
    input.BindCommand(SDLK_LEFT, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::DownLeft));
    input.BindCommand(SDLK_DOWN, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::DownRight));
}

void InputBindingHelper::BindPlayer1GamepadInputs(dae::GameObject* qbert)
{
    auto& input = dae::InputManager::GetInstance();

    input.BindCommand(0, GamepadButton::DPadUp, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::UpLeft));
    input.BindCommand(0, GamepadButton::DPadRight, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::UpRight));
    input.BindCommand(0, GamepadButton::DPadLeft, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::DownLeft));
    input.BindCommand(0, GamepadButton::DPadDown, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::DownRight));
}

void InputBindingHelper::BindPlayer2KeyboardInputs(dae::GameObject* qbert)
{
    auto& input = dae::InputManager::GetInstance();

    input.BindCommand(SDLK_w, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::UpLeft));
    input.BindCommand(SDLK_d, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::UpRight));
    input.BindCommand(SDLK_a, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::DownLeft));
    input.BindCommand(SDLK_s, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::DownRight));
}

void InputBindingHelper::BindPlayer2GamepadInputs(dae::GameObject* qbert)
{
    auto& input = dae::InputManager::GetInstance();

    input.BindCommand(0, GamepadButton::DPadUp, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::UpLeft));
    input.BindCommand(0, GamepadButton::DPadRight, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::UpRight));
    input.BindCommand(0, GamepadButton::DPadLeft, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::DownLeft));
    input.BindCommand(0, GamepadButton::DPadDown, KeyState::Down, std::make_shared<dae::MoveCommand>(qbert, D::DownRight));
}

void InputBindingHelper::BindPlayer2CoilyGamepadInputs(dae::GameObject* coily)
{
    auto& input = dae::InputManager::GetInstance();

    input.BindCommand(0, GamepadButton::DPadUp, KeyState::Down, std::make_shared<dae::MoveCommand>(coily, D::UpLeft));
    input.BindCommand(0, GamepadButton::DPadRight, KeyState::Down, std::make_shared<dae::MoveCommand>(coily, D::UpRight));
    input.BindCommand(0, GamepadButton::DPadLeft, KeyState::Down, std::make_shared<dae::MoveCommand>(coily, D::DownLeft));
    input.BindCommand(0, GamepadButton::DPadDown, KeyState::Down, std::make_shared<dae::MoveCommand>(coily, D::DownRight));
}

