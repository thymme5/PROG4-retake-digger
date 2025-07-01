#include "Gamepad.h"

dae::Gamepad::Gamepad(int index) : m_Index(index)
{
    ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
    ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
}

void dae::Gamepad::Update()
{
    m_PreviousState = m_CurrentState;
    ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
    XInputGetState(m_Index, &m_CurrentState);
}

bool dae::Gamepad::IsButtonDown(WORD button) const
{
    return (m_CurrentState.Gamepad.wButtons & button) && !(m_PreviousState.Gamepad.wButtons & button);
}

bool dae::Gamepad::IsButtonUp(WORD button) const
{
    return !(m_CurrentState.Gamepad.wButtons & button) && (m_PreviousState.Gamepad.wButtons & button);
}

bool dae::Gamepad::IsButtonPressed(WORD button) const
{
    return m_CurrentState.Gamepad.wButtons & button;
}

