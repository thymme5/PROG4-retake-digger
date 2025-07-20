#include "DiggerSceneBuilder.h"

void DiggerSceneBuilder::CreateBaseDiggerScene(dae::Scene& scene, const std::string& levelPath)
{
	LevelBuilder::LoadLevelFromFile(levelPath, scene);
}

void DiggerSceneBuilder::CreateSinglePlayerScene(dae::Scene& scene, const std::string& levelPath)
{
	CreateBaseDiggerScene(scene, levelPath);
}

