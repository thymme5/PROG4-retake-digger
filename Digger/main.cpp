#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif
#include <iostream>

// Engine files
#include "Minigin.h"
#include "InputManager.h"

// Game files
#include "GameModeManager.h"
#include "MainMenu.h"
#include "DiggerSoundLibrary.h"

// STD library files
#include <filesystem>v

void load()
{
    DiggerSoundLibrary::LoadAllSounds();
	GameModeManager::GetInstance().SetMode(std::make_unique<MainMenu>());
}

int main(int, char* argv[])
{
    std::filesystem::path exeDir = std::filesystem::canonical(argv[0]).parent_path();
    std::filesystem::current_path(exeDir);

    std::filesystem::path dataPath = exeDir / "Data";
    dae::Minigin engine(dataPath.string());

    engine.Run(load);
    return 0;
}