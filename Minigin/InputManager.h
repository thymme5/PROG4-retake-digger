#pragma once
#include "Singleton.h"
#include "Command.h"
#include "gamepad.h"
#include "SDL.h"

#include <unordered_map>
#include <memory>

enum class KeyState
{
	Down,
	Up,
	Pressed
};
//TODO: GamepadButton is not entirely written by the rules of PIMPL, fix it
namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager();

		bool ProcessInput();
		void BindCommand(SDL_Keycode key, KeyState state, std::shared_ptr<Command> command);
		void BindCommand(int playerIdx, GamepadButton gamepadButton, KeyState state, std::shared_ptr<Command> command);

		void ClearAllBindings();

	private:
		class Impl;
		std::unique_ptr<Impl> m_Impl;
	};
}
