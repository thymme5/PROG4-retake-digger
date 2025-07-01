#pragma once
#include "Command.h"
#include "QbertMoveComponent.h"
#include "CoilyComponent.h"
namespace dae
{
    class MoveCommand : public Command
    {
    public:
        MoveCommand(GameObject* pGameObject, Direction dir)
            : m_pGameObject(pGameObject), m_Direction(dir)
        {

        }

        void Execute() override
        {
            // supports both coily and qbert (I hope theres no ambiguity here)
            if (m_pGameObject->HasComponent<QbertMoveComponent>())
            {
                m_pGameObject->GetComponent<QbertMoveComponent>()->TryMove(m_Direction);
            }
            else if (m_pGameObject->HasComponent<CoilyComponent>())
            {
                auto* coilyComponent = m_pGameObject->GetComponent<CoilyComponent>();
            
                // only allow player input when in ChasingState
                if (dynamic_cast<const ChasingState*>(coilyComponent->GetState()))
                {
                    coilyComponent->TryMove(m_Direction);
                }
            }
        }

    private:
        GameObject* m_pGameObject{};
        Direction m_Direction{};
    };
}
