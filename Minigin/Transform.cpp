#include "Transform.h"
#include "GameObject.h"
#include "glm.hpp"
#include <cmath>

dae::Transform::Transform(GameObject* parent)
    : m_pParent{ parent }
{
}

void dae::Transform::SetParent(GameObject* parent)
{
    m_pParent = parent;
    SetDirty();
}

const glm::vec3& dae::Transform::GetLocalPosition() const
{
    return m_LocalPosition;
}

void dae::Transform::SetLocalPosition(float x, float y, float z)
{
    m_LocalPosition = { x, y, z };
    SetDirty();
}

void dae::Transform::SetLocalPosition(const glm::vec3& pos)
{
    m_LocalPosition = pos;
    SetDirty();
}

const glm::vec3& dae::Transform::GetWorldPosition()
{
    if (m_IsPositionDirty)
    {
        UpdateWorldPosition();
    }
    return m_WorldPosition;
}

void dae::Transform::UpdateWorldPosition()
{
    if (m_pParent == nullptr)
    {
        m_WorldPosition = m_LocalPosition;
    }
    else
    {
        //TODO: dirty flag is positioned incorrect, but will crash otherwise due to recursion
        m_IsPositionDirty = false;
        m_WorldPosition = m_pParent->GetTransform().GetWorldPosition() + m_LocalPosition;
    }
}

void dae::Transform::SetDirty()
{
    m_IsPositionDirty = true;
}

void dae::Transform::Rotate(float z)
{
    float cosTheta = cosf(z);
    float sinTheta = sinf(z);

    glm::vec3 rotatedPos;
    rotatedPos.x = m_LocalPosition.x * cosTheta - m_LocalPosition.y * sinTheta;
    rotatedPos.y = m_LocalPosition.x * sinTheta + m_LocalPosition.y * cosTheta;
    rotatedPos.z = m_LocalPosition.z;

    SetLocalPosition(rotatedPos);
}

void dae::Transform::Translate(float x, float y, float z)
{
    m_LocalPosition += glm::vec3(x, y, z);
    SetDirty();
}
