#pragma once
#include "Component.h"

class InteractableComponent : public dae::Component
{
public:
    using Component::Component;

    virtual void Interact(dae::GameObject& interactor) = 0;
};
