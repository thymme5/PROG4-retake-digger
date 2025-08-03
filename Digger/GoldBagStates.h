#pragma once
#include <memory>

class GoldBagComponent;

// === Base class ====
class GoldBagState
{
public:
    virtual ~GoldBagState() = default;
    virtual void Enter(GoldBagComponent& goldBag) {}
    virtual void Update(GoldBagComponent& goldBag) = 0;
    virtual void Exit(GoldBagComponent& goldBag) {}
};

// === Falling State ===
class FallingState : public GoldBagState
{
public:
    void Enter(GoldBagComponent& goldBag) override;
    void Update(GoldBagComponent& goldBag) override;
    void Exit(GoldBagComponent& goldBag) override;
};

// === Idle State ===
class IdleState : public GoldBagState
{
public:
    void Enter(GoldBagComponent& goldBag) override;
    void Update(GoldBagComponent& goldBag) override;
    void Exit(GoldBagComponent& goldBag) override;
};

// === Broken (Collectable) State ===
class BrokenState : public GoldBagState
{
public:
    void Enter(GoldBagComponent& goldBag) override;
    void Update(GoldBagComponent& goldBag) override;
    void Exit(GoldBagComponent& goldBag) override;
};
