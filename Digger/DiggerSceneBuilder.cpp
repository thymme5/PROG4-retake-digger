#include "DiggerSceneBuilder.h"
#include "Scene.h"

void DiggerSceneBuilder::CreateBaseDiggerScene(dae::Scene& scene, const std::string& levelPath)
{
	LevelBuilder::LoadLevelFromFile(levelPath, scene);
}

void DiggerSceneBuilder::CreateSinglePlayerScene(dae::Scene& scene, const std::string& levelPath)
{
	CreateBaseDiggerScene(scene, levelPath);
}

void DiggerSceneBuilder::CreateDebugScene(dae::Scene& scene)
{
    constexpr int TILE_SIZE = 48;
    constexpr int SCREEN_WIDTH = 960;
    constexpr int SCREEN_HEIGHT = 560;

    const int cols = SCREEN_WIDTH / TILE_SIZE;
    const int rows = SCREEN_HEIGHT / TILE_SIZE;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            std::string texture = (row + col) % 2 == 0 ? "tile_white.png" : "tile_black.png";
            auto tile = std::make_shared<dae::GameObject>();
            tile->SetPosition(col * TILE_SIZE, row * TILE_SIZE);
            tile->AddComponent<dae::TextureComponent>(*tile, texture, 1.5f, 0);
            scene.Add(tile);
        }
    }
}
