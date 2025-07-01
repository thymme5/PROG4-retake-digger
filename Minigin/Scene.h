#pragma once
#include "SceneManager.h"
#include "ImGuiComponent.h"
#include "GameObject.h"

#include <memory>
#include <vector>
#include <algorithm>

namespace dae
{
	class GameObject;
	class Component;

	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);

		void RemoveAll();

		template <typename T>
		void RemoveObjectsWithComponent()
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

			m_objects.erase(
				std::remove_if(m_objects.begin(), m_objects.end(),
					[](const std::shared_ptr<GameObject>& obj) -> bool
					{
						return obj->HasComponent<T>();
					}),
				m_objects.end());

		}
		template <typename T>
		std::shared_ptr<dae::GameObject> FindFirstObjectOfType()
		{
			for (const auto& go : m_objects)
			{
				if (go->GetComponent<T>() != nullptr)
					return go;
			}
			return nullptr;
		}
		template <typename T>
		std::vector<std::shared_ptr<dae::GameObject>> FindObjectsOfType()
		{
			static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

			std::vector<std::shared_ptr<dae::GameObject>> results;

			for (const auto& go : m_objects)
			{
				if (go->GetComponent<T>() != nullptr)
					results.push_back(go);
			}

			return results;
		}
		void Update();
		void Render() const;
		void RenderImGui();
		std::string GetName() const { return m_name; };

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

}
