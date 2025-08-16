#pragma once
#include "Command.h"
#include "NameEntryComponent.h"

class ChangeLetterCommand final : public dae::Command
{
public:
    ChangeLetterCommand(NameEntryComponent* component, int direction)
        : m_Component(component), m_Direction(direction) {
    }

    void Execute() override
    {
        if (m_Component)
            m_Component->ChangeLetter(m_Direction);
    }

private:
    NameEntryComponent* m_Component;
    int m_Direction;
};
