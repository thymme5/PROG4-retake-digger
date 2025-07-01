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

#include "GameModeManager.h"
#include "MainMenu.h"
#include "SoundService.h"
#include "ResourceManager.h"

#include <filesystem>

void load()
{
	// build main menu
	GameModeManager::GetInstance().SetMode(std::make_unique<MainMenu>());
}
int main(int, char* [])
{
    dae::Minigin engine("Data/");
	engine.Run(load);
	return 0;
}