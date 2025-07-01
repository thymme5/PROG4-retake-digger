#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}
void dae::SceneManager::PopScene()
{
	m_scenes.pop_back();
}
void dae::SceneManager::RemoveScene(const std::string& name)
{
	for (auto it = m_scenes.begin(); it != m_scenes.end(); ++it)
	{
		if ((*it)->GetName() == name)
		{
			m_scenes.erase(it);
			break;
		}
	}
}
void dae::SceneManager::MarkSceneForDeletion(const std::string& name)
{
	m_sceneMarkedForDeletion = name;
}
void dae::SceneManager::Cleanup()
{
	if (!m_sceneMarkedForDeletion.empty())
	{
		RemoveScene(m_sceneMarkedForDeletion);
		m_sceneMarkedForDeletion.clear();
	}
}
void dae::SceneManager::RenderUI()
{
	for (auto& scene : m_scenes)
	{
		scene->RenderImGui();
	}
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
