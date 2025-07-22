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

	//TODO: clean this up and put everything in it's proper place
	std::stringstream ss;
	ss << dae::ResourceManager::GetInstance().GetDataPath().string() << "levels/Level01Solo.json";
	std::string levelPath = ss.str();

	DiggerSceneBuilder::CreateSinglePlayerScene(dae::SceneManager::GetInstance().CreateScene("mainscene"), levelPath);
	//DiggerSceneBuilder::CreateDebugScene(dae::SceneManager::GetInstance().CreateScene("mainscene"));
}
int main(int, char* [])
{
	dae::Minigin engine("Data/");
	engine.Run(load);
	return 0;
}