#pragma once
#include "Command.h"
#include "NameEntryComponent.h"

class ConfirmNameCommand final : public dae::Command
{
public:
    explicit ConfirmNameCommand(NameEntryComponent* component)
        : m_Component(component) {
    }

    void Execute() override
    {
        if (m_Component)
            m_Component->Confirm();
    }

private:
    NameEntryComponent* m_Component;
};
