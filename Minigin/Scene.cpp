#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::CleanupDestroyedObjects()
{
	m_objects.erase(
		std::remove_if(m_objects.begin(), m_objects.end(),
			[](const std::shared_ptr<GameObject>& obj)
			{
				return obj->IsMarkedForDestroy();
			}),
		m_objects.end()
	);
}

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object->Update();
	}

	CleanupDestroyedObjects();
}

void Scene::Render() const
{
	std::vector<const dae::Component*> renderQueue;

	// Collect all renderable components from all GameObjects
	for (const auto& object : m_objects)
	{
		for (const auto& comp : object->GetComponents())
		{
			renderQueue.push_back(comp.get());
		}
	}

	// Sort components by their render layer
	std::sort(renderQueue.begin(), renderQueue.end(),
		[](const dae::Component* a, const dae::Component* b)
		{
			return static_cast<int>(a->GetRenderLayer()) < static_cast<int>(b->GetRenderLayer());
		});

	// Render in sorted order
	for (const auto& component : renderQueue)
	{
		component->Render();
	}
}


void Scene::RenderImGui()
{
	for (auto& object : m_objects)
	{
		if (object->GetComponent<ImGuiComponent>())
		{
			object->RenderImGui();
		}
	}
}
