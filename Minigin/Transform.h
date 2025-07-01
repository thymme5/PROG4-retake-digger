#pragma once

#include "glm.hpp"

namespace dae
{
	class Transform final
	{
	public:
		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);

		const glm::vec3& GetRotation() const { return m_rotationAngles; }
		void SetRotation(const glm::vec3& rotation) { m_rotationAngles = rotation; }
	private:
		glm::vec3 m_position;
		glm::vec3 m_rotationAngles; //rot angles in pitch yaw & roll
	};
}
