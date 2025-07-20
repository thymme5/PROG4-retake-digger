#include "LevelBuilder.h"
#include "TextureComponent.h"
#include "Scene.h"
#include <fstream>
#include <iostream>

constexpr int TILE_SIZE = 16;

void LevelBuilder::LoadLevelFromFile(const std::string& path, dae::Scene& scene)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open level file: " << path << '\n';
        return;
    }

    std::string line;
    int row = 0;

    while (std::getline(file, line))
    {
        for (int col = 0; col < static_cast<int>(line.size()); ++col)
        {
            std::pair<int, int> gridPos{ col, row };
            char tile = line[col];

            switch (tile)
            {
            case '#': SpawnWall(scene, gridPos); break;
            case 'E': SpawnEmerald(scene, gridPos); break;
            case 'G': SpawnGoldBag(scene, gridPos); break;
            case 'P': SpawnPlayer(scene, gridPos); break;
            case 'N': SpawnNobbin(scene, gridPos); break;
            case 'H': SpawnHobbin(scene, gridPos); break;
            case '.': /* Empty tunnel, no spawn needed */ break;
            default:
                std::cerr << "Unknown tile character: " << tile << " at (" << col << ", " << row << ")\n";
                break;
            }
        }
        ++row;
    }
}

std::shared_ptr<dae::GameObject> LevelBuilder::CreateBasicGO(dae::Scene& scene, std::pair<int, int> gridPos, const std::string& texturePath)
{
    auto go = std::make_shared<dae::GameObject>();
    go->SetPosition(gridPos.first * TILE_SIZE, gridPos.second * TILE_SIZE);

    go->AddComponent<dae::TextureComponent>(*go, texturePath, 1.f, 0);

    scene.Add(go);
    return go;
}
void LevelBuilder::SpawnWall(dae::Scene& scene, std::pair<int, int> gridPos)
{
    //auto go = CreateBasicGO(scene, gridPos, "Wall.png");
    // go->AddComponent<WallComponent>(*go);
}

void LevelBuilder::SpawnEmerald(dae::Scene& scene, std::pair<int, int> gridPos)
{
    auto go = CreateBasicGO(scene, gridPos, "Emerald.png");
    // go->AddComponent<EmeraldComponent>(*go);
}

void LevelBuilder::SpawnGoldBag(dae::Scene& scene, std::pair<int, int> gridPos)
{
    auto go = CreateBasicGO(scene, gridPos, "goldbag.png");
    // go->AddComponent<GoldBagComponent>(*go);
}

void LevelBuilder::SpawnPlayer(dae::Scene& scene, std::pair<int, int> gridPos)
{
    auto go = CreateBasicGO(scene, gridPos, "digger.png");
    // go->AddComponent<PlayerComponent>(*go);
}

void LevelBuilder::SpawnNobbin(dae::Scene& scene, std::pair<int, int> gridPos)
{
    //auto go = CreateBasicGO(scene, gridPos, "Nobbin.png");
    // go->AddComponent<NobbinComponent>(*go);
}

void LevelBuilder::SpawnHobbin(dae::Scene& scene, std::pair<int, int> gridPos)
{
    //auto go = CreateBasicGO(scene, gridPos, "Hobbin.png");
    // go->AddComponent<HobbinComponent>(*go);
}
