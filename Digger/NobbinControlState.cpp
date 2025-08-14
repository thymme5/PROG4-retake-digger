#include "NobbinControlState.h"
#include "EnemyComponent.h"
#include "InputManager.h"
#include "MoveEnemyCommand.h"

void NobbinControlState::Enter(EnemyComponent& enemy)
{
	std::cout << "[NobbinControlState] Entering control state for enemy at ("
		<< enemy.GetRow() << ", " << enemy.GetCol() << ")\n";
    auto& input = dae::InputManager::GetInstance();

    input.BindCommand(0, GamepadButton::DPadUp, KeyState::Held, std::make_shared<MoveEnemyCommand>(enemy, -1, 0));
    input.BindCommand(0, GamepadButton::DPadDown, KeyState::Held, std::make_shared<MoveEnemyCommand>(enemy, 1, 0));
    input.BindCommand(0, GamepadButton::DPadLeft, KeyState::Held, std::make_shared<MoveEnemyCommand>(enemy, 0, -1));
    input.BindCommand(0, GamepadButton::DPadRight, KeyState::Held, std::make_shared<MoveEnemyCommand>(enemy, 0, 1));
}

void NobbinControlState::Update(EnemyComponent& enemy)
{

}
