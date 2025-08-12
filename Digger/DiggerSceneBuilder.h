#pragma once
#include <memory>
#include <string>
#include "LevelBuilder.h"

namespace dae
{
	class Scene;
}

class DiggerSceneBuilder final
{
public:
	static void CreateBaseDiggerScene(dae::Scene& scene, const std::string& levelPath);
	
	static void BuildMainMenu(dae::Scene& scene, const std::shared_ptr<dae::Font>& font);
	static void CreateSinglePlayerScene(dae::Scene& scene, const std::string& levelPath);
	static void CreateCoopScene();
	static void CreateVersusScene();


	static void CreateDebugScene(dae::Scene& scene);
};
