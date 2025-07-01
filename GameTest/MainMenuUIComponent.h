#pragma once
#include "Component.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "TextComponent.h"

class MainMenuUIComponent final : public dae::Component
{
public:
    MainMenuUIComponent(dae::GameObject& owner);

    void SetArrow(const std::shared_ptr<dae::GameObject>& arrow);
    void MoveArrow(float directionSign);

    void Update() override;
    void Render() const override;
    
    int GetSelectedIndex() { return m_SelectedIndex; };

private:
    std::shared_ptr<dae::Texture2D> m_LogoTexture;

    std::shared_ptr<dae::TextComponent> m_pSoloText;
    std::shared_ptr<dae::TextComponent> m_pCoopText;
    std::shared_ptr<dae::TextComponent> m_pVersus;

    int m_SelectedIndex{ 0 };
    float m_ArrowBaseY;
    float m_ArrowX;

    std::weak_ptr<dae::GameObject> m_pArrow;
};

