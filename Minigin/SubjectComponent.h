#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
    class SubjectComponent final : public Component, public Subject
    {
    public:
        SubjectComponent(GameObject& owner)
            : Component(owner)
        {
        }
        ~SubjectComponent() override = default;

        void Update() override {};
        void Render() const override {};
    };
}
