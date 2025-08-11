// UIComponent.h
#pragma once
#include <vector>
#include <string>
#include "Component.h"
#include "Observer.h"   // Event enum + Observer base

namespace dae { class SubjectComponent; class TextComponent; class GameObject; }

class UIComponent final : public dae::Component, public dae::Observer
{
public:
    explicit UIComponent(dae::GameObject& owner);
    ~UIComponent() override { StopObservingAll(); }

    void Observe(dae::SubjectComponent& subject);
    void StopObservingAll();

    void OnNotify(dae::Event event, dae::GameObject* pGameObject) override;

    void Update() override;
    void Render() const override {}

private:
    // HUD state
    int  m_Score{ 0 };
    int  m_Lives{ 4 };
    int  m_Level{ 1 };
    int  m_NextLifeScore{ 20000 };

    bool m_Dirty{ true };

    dae::TextComponent* m_pText{ nullptr };

    std::vector<dae::SubjectComponent*> m_Sources;

    void EnsureTextCached();
    void RefreshText();
};