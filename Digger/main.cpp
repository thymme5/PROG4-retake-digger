#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include <iostream>

//engine files
#include "Minigin.h"
#include "Scene.h"
#include "InputManager.h"

#include "SoundService.h"
#include "ResourceManager.h"
#include "LevelBuilder.h"
#include "DiggerSceneBuilder.h"
#include <filesystem>
#include <sstream>

void load()
{
	std::stringstream ss;
	ss << dae::ResourceManager::GetInstance().GetDataPath().string() << "levels/level01test.txt";
	std::string levelPath = ss.str();

	DiggerSceneBuilder::CreateSinglePlayerScene(dae::SceneManager::GetInstance().CreateScene("mainscene"), levelPath);
}
int main(int, char* [])
{
	dae::Minigin engine("Data/");
	engine.Run(load);
	return 0;
}