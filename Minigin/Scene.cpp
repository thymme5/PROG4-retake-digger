#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

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
}

void Scene::Render() const
{
	// render objects based on their layer
	auto sortedObjects = m_objects;
	std::sort(sortedObjects.begin(), sortedObjects.end(),
		[](const std::shared_ptr<GameObject>& a, const std::shared_ptr<GameObject>& b)
		{
			return static_cast<int>(a->GetRenderLayer()) < static_cast<int>(b->GetRenderLayer());
		});

	for (const auto& obj : sortedObjects)
		obj->Render();
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