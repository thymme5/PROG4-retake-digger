#pragma once
#include "Component.h"

class FireballComponent : public dae::Component
{
public:
    FireballComponent(dae::GameObject& owner, int row, int col, int dRow, int dCol);
    void Update() override;
    void Render() const override;

private:
    // position
    int m_Row;
    int m_Col;

    // Direction
    int m_DirRow;
    int m_DirCol;

    // Movement interpolation
    bool m_IsMoving = false;
    int m_TargetRow = 0;
    int m_TargetCol = 0;
    float m_MoveSpeed = 200.0f;
    float m_MoveTimer = 0.0f;

    void StartNextMovement();
    void CheckEnemyCollision();
};