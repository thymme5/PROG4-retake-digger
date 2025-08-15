#pragma once
#include <string>
#include "Component.h"

namespace dae { class TextComponent; class GameObject; }

class UIComponent final : public dae::Component
{
public:
    explicit UIComponent(dae::GameObject& owner);

    void Update() override;
    void Render() const override {}

private:
    int  m_Score{ 0 };
    int  m_Lives{ 0 };
    int  m_Level{ 1 };

    bool m_Dirty{ true };

    dae::TextComponent* m_pText{ nullptr };

    void EnsureTextCached();
    void RefreshText();
};
