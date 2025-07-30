#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <glm/vec3.hpp>
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

		void SetLocalPosition(float x, float y);
		const glm::vec3& GetLocalPosition() const;
		glm::vec3 GetWorldPosition();
		Transform& GetTransform();

		void SetParent(GameObject* newParent, bool keepWorldPosition = true);
		GameObject* GetParent() const noexcept;
		GameObject* GetChildAt(int index);
		int GetChildCount() const noexcept;

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
		void Reparent(GameObject* newParent, bool keepWorldPosition);
		void MarkDirty();

		Transform m_Transform;

		GameObject* m_Parent{ nullptr };
		std::vector<GameObject*> m_Children;
		std::vector<std::unique_ptr<Component>> m_Components;

		bool m_IsDirty{ true };
	};
}
