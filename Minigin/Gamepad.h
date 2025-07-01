#pragma once
#include <windows.h>
#include "Xinput.h"

//Due to ambiguity between SDLK and XINPUT's keybinds, i've decided to essentially 'wrap' the XINPUT's input in a enum class 
enum class GamepadButton
{
    A = XINPUT_GAMEPAD_A,
    B = XINPUT_GAMEPAD_B,
    X = XINPUT_GAMEPAD_X,
    Y = XINPUT_GAMEPAD_Y,
    DPadUp = XINPUT_GAMEPAD_DPAD_UP,
    DPadDown = XINPUT_GAMEPAD_DPAD_DOWN,
    DPadLeft = XINPUT_GAMEPAD_DPAD_LEFT,
    DPadRight = XINPUT_GAMEPAD_DPAD_RIGHT,
    LeftShoulder = XINPUT_GAMEPAD_LEFT_SHOULDER,
    RightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
    Start = XINPUT_GAMEPAD_START,
    Back = XINPUT_GAMEPAD_BACK,
    LeftThumb = XINPUT_GAMEPAD_LEFT_THUMB,
    RightThumb = XINPUT_GAMEPAD_RIGHT_THUMB
};

namespace dae
{
    class Gamepad
    {
    public:
        explicit Gamepad(int index);
        void Update();
        bool IsButtonDown(WORD button) const;
        bool IsButtonUp(WORD button) const;
        bool IsButtonPressed(WORD button) const;

    private:
        int m_Index;
        XINPUT_STATE m_CurrentState{};
        XINPUT_STATE m_PreviousState{};
    };
}
