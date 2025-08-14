#pragma once
#include "EnemyState.h"
#include <glm/glm.hpp>

class NobbinControlState final : public EnemyState
{
public:
    void Enter(EnemyComponent& enemy) override;
    void Update(EnemyComponent& enemy) override;
    void Exit(EnemyComponent& enemy) override {}
};
