#include "Gamepad.h"
#include <iostream>

namespace dae
{
    Gamepad::Gamepad(int index)
        : m_Index(index)
    {
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
    }

    void Gamepad::Update()
    {
        m_PreviousState = m_CurrentState;
        ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
        XInputGetState(m_Index, &m_CurrentState);
    }

    bool Gamepad::IsButtonJustPressed(WORD button) const
    {
        return (m_CurrentState.Gamepad.wButtons & button) &&
            !(m_PreviousState.Gamepad.wButtons & button);
    }

    bool Gamepad::IsButtonHeld(WORD button) const
    {
        return (m_CurrentState.Gamepad.wButtons & button);
    }

    bool Gamepad::IsButtonJustReleased(WORD button) const
    {
        return !(m_CurrentState.Gamepad.wButtons & button) &&
            (m_PreviousState.Gamepad.wButtons & button);
    }
}
