#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);
		void RemoveScene(const std::string& name);
		void PopScene();
		void MarkSceneForDeletion(const std::string& name);
		void Cleanup();
		void Update();
		void Render();
		void RenderUI();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::string m_sceneMarkedForDeletion;
	};
}
