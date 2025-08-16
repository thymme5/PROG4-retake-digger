#pragma once
#include "Command.h"
#include "NameEntryComponent.h"

class MoveCursorCommand final : public dae::Command
{
public:
    MoveCursorCommand(NameEntryComponent* component, int direction)
        : m_Component(component), m_Direction(direction) {
    }

    void Execute() override
    {
        if (m_Component)
            m_Component->MoveCursor(m_Direction);
    }

private:
    NameEntryComponent* m_Component;
    int m_Direction;
};
