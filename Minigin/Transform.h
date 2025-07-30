#pragma once
#include <glm/vec3.hpp>

namespace dae
{
	class Transform
	{
	public:
		void SetPosition(const glm::vec3& pos);
		void SetPosition(float x, float y, float z);

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetWorldPosition() const;
		void SetWorldPosition(const glm::vec3& pos);

	private:
		glm::vec3 m_LocalPosition{};
		glm::vec3 m_WorldPosition{};
	};
}
