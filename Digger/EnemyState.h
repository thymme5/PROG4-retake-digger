#pragma once
#include <memory>

class EnemyComponent;

class EnemyState
{
public:
    virtual ~EnemyState() = default;

    virtual void Enter(EnemyComponent& enemy) {}
    virtual void Update(EnemyComponent& enemy) = 0;
    virtual void Exit(EnemyComponent& enemy) {}
};
