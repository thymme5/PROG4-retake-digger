#pragma once
#include <glm.hpp>

namespace dae
{
    class GameObject;

    class Transform final
    {
    public:
        explicit Transform(GameObject* parent);

        void SetParent(GameObject* parent);

        const glm::vec3& GetLocalPosition() const;
        void SetLocalPosition(float x, float y, float z);
        void SetLocalPosition(const glm::vec3& pos);

        const glm::vec3& GetWorldPosition();

        void Rotate(float z);
        void Translate(float x, float y, float z);

        void UpdateWorldPosition();
        void SetDirty();

    private:
        GameObject* m_pParent{ nullptr };

        glm::vec3 m_WorldPosition{ 0.0f };
        glm::vec3 m_LocalPosition{ 0.0f };

        bool m_IsPositionDirty{ true };
    };
}
