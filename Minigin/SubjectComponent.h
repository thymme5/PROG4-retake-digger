#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
    class SubjectComponent final : public Component, public Subject
    {
    public:
        SubjectComponent() = default;
        ~SubjectComponent() override = default;
    };
}
