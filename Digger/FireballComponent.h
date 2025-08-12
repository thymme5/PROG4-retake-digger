#pragma once
#include "Component.h"

class FireballComponent : public dae::Component
{
public:
    FireballComponent(dae::GameObject& owner, int row, int col, int dRow, int dCol);

    void Update() override;
    void Render() const override;
private:
    int m_Row{}, m_Col{};
    int m_DirRow{}, m_DirCol{};
    float m_Timer = 0.f;
    float m_MoveInterval = 3.f;

};
