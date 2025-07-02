#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <glm.hpp>
#include "Transform.h"
#include "Component.h"
#include "ImGuiComponent.h"

namespace dae
{
    class GameObject final
    {
    public:
        GameObject();
        ~GameObject();
        GameObject(GameObject&& other) noexcept = default;
        GameObject& operator=(GameObject&& other) noexcept = default;
        GameObject(const GameObject& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;

        void Update();
        void Render() const;
        void RenderImGui();

        void SetPosition(float x, float y);
        glm::vec3 GetWorldPosition();
        void UpdateWorldPosition();

        Transform& GetTransform();

        void SetParent(GameObject* parent);
        GameObject* GetParent() const noexcept;
        GameObject* GetChildAt(int index);
        int GetChildCount() const noexcept;

        // component management
        template <typename T, typename... Args>
        T* AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            auto component = std::make_unique<T>(std::forward<Args>(args)...);
            T* rawPtr = component.get();
            m_Components.emplace_back(std::move(component));
            return rawPtr;
        }

        template <typename T>
        void RemoveComponent()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            m_Components.erase(
                std::remove_if(m_Components.begin(), m_Components.end(),
                    [](const std::unique_ptr<Component>& component) {
                        return dynamic_cast<T*>(component.get()) != nullptr;
                    }),
                m_Components.end());
        }

        template <typename T>
        T* GetComponent() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            for (const auto& component : m_Components)
            {
                if (auto casted = dynamic_cast<T*>(component.get()))
                    return casted;
            }
            return nullptr;
        }

        template <typename T>
        bool HasComponent() const
        {
            static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");
            for (const auto& component : m_Components)
            {
                if (dynamic_cast<T*>(component.get()))
                    return true;
            }
            return false;
        }

        // search for a child with a specific component
        template<typename T>
        GameObject* FindChildWithComponent()
        {
            for (auto* child : m_Children)
            {
                if (child && child->GetComponent<T>())
                    return child;
            }
            return nullptr;
        }

    private:
        Transform m_transform;

        GameObject* m_Parent{ nullptr };
        std::vector<GameObject*> m_Children;
        std::vector<std::unique_ptr<Component>> m_Components;
    };
}
