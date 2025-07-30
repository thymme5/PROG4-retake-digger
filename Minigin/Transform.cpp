#include "Transform.h"

namespace dae
{
	void Transform::SetPosition(const glm::vec3& pos)
	{
		m_LocalPosition = pos;
	}

	void Transform::SetPosition(float x, float y, float z)
	{
		m_LocalPosition = { x, y, z };
	}

	const glm::vec3& Transform::GetLocalPosition() const
	{
		return m_LocalPosition;
	}

	const glm::vec3& Transform::GetWorldPosition() const
	{
		return m_WorldPosition;
	}

	void Transform::SetWorldPosition(const glm::vec3& pos)
	{
		m_WorldPosition = pos;
	}
}
