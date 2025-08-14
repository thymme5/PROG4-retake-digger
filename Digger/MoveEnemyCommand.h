#pragma once
#pragma once
#include "Command.h"
#include "EnemyComponent.h"

class MoveEnemyCommand final : public dae::Command
{
public:
    MoveEnemyCommand(EnemyComponent& enemy, int dRow, int dCol)
        : m_pEnemy{ &enemy }, m_DeltaRow{ dRow }, m_DeltaCol{ dCol }
    {

    }

    void Execute() override
    {
        if (m_pEnemy && m_pEnemy->IsPlayerControlled())
            m_pEnemy->MoveBy(m_DeltaRow, m_DeltaCol);
    }

private:
    EnemyComponent* m_pEnemy{};
    int m_DeltaRow{};
    int m_DeltaCol{};
};
