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
    auto& rm = dae::ResourceManager::GetInstance();

    std::filesystem::path levelPath = rm.GetDataPath() / "levels" / "Level01Solo.json";

    if (!std::filesystem::exists(levelPath)) {
        std::cerr << "Failed to open level file: " << levelPath.string() << "\n";
        return;
    }

    DiggerSceneBuilder::CreateSinglePlayerScene(dae::SceneManager::GetInstance().CreateScene("mainscene"), levelPath.string());
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