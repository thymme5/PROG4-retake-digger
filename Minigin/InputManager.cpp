
#include <SDL.h>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Gamepad.h"
#include "InputManager.h"

using namespace dae;
class InputManager::Impl
{
public:
    Impl()
    {
        // Initialize up to 4 gamepads (index 0-3)
        for (int i = 0; i < 4; ++i)
        {
            m_Gamepads.push_back(std::make_unique<Gamepad>(i));
        }

        m_ClearedInput = false;
    }

    bool ProcessInput()
    {
        m_ClearedInput = false;

        // Update all connected gamepads
        for (auto& gamepad : m_Gamepads)
        {
            if (gamepad)
                gamepad->Update();
        }

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                return false;
            }

            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
            {
                auto key = e.key.keysym.sym;
                KeyState state = (e.type == SDL_KEYDOWN) ? KeyState::Down : KeyState::Up;

                if (m_KeyCommands.find(key) != m_KeyCommands.end())
                {
                    for (const auto& [boundState, command] : m_KeyCommands[key])
                    {
                        if (boundState == state)
                        {
                            command->Execute();
                        }
                    }
                }
            }
        }

        // Handle gamepad input per player index
        for (size_t playerIdx = 0; playerIdx < m_Gamepads.size(); ++playerIdx)
        {
            auto& gamepad = m_Gamepads[playerIdx];
            if (!gamepad)
                continue;

            //explicit casting is safe here so i'm just doing it
            const auto& buttonMap = m_ControllerCommands[static_cast<int>(playerIdx)];
            for (const auto& [button, commands] : buttonMap)
            {
                if (m_ClearedInput) return true;

                for (const auto& [state, command] : commands)
                {
                    if (IsButtonState(*gamepad, button, state))
                    {
                        command->Execute();
                    }
                }
            }
        }
        
        return true;
    }

    void BindCommand(SDL_Keycode key, KeyState state, std::shared_ptr<Command> command)
    {
        m_KeyCommands[key].emplace_back(state, command);
    }

    void BindCommand(int playerIdx, WORD button, KeyState state, std::shared_ptr<Command> command)
    {
        m_ControllerCommands[playerIdx][button].emplace_back(state, command);
    }

    void ClearAll()
    {
        m_KeyCommands.clear();
        m_ControllerCommands.clear();
        m_ClearedInput = true;
        
    }

private:
    bool IsButtonState(Gamepad& gamepad, WORD button, KeyState state) const
    {
        switch (state)
        {
        case KeyState::Down:
            return gamepad.IsButtonDown(button);
        case KeyState::Up:
            return gamepad.IsButtonUp(button);
        case KeyState::Pressed:
            return gamepad.IsButtonPressed(button);
        default:
            return false;
        }
    }

    std::vector<std::unique_ptr<Gamepad>> m_Gamepads;
    std::unordered_map<SDL_Keycode, std::vector<std::pair<KeyState, std::shared_ptr<Command>>>> m_KeyCommands;
    std::unordered_map<int, std::unordered_map<WORD, std::vector<std::pair<KeyState, std::shared_ptr<Command>>>>> m_ControllerCommands;
    bool m_ClearedInput;
};

InputManager::InputManager() : m_Impl(std::make_unique<Impl>()) {}
InputManager::~InputManager() = default;

bool InputManager::ProcessInput()
{
    return m_Impl->ProcessInput();
}

void InputManager::BindCommand(SDL_Keycode key, KeyState state, std::shared_ptr<Command> command)
{
    m_Impl->BindCommand(key, state, command);
}

void InputManager::BindCommand(int playerIdx, GamepadButton button, KeyState state, std::shared_ptr<Command> command)
{
    m_Impl->BindCommand(playerIdx, static_cast<WORD>(button), state, command);
}

void InputManager::ClearAllBindings()
{
    m_Impl->ClearAll();
}
