#include "LevelBuilder.h"
#include "TextureComponent.h"
#include "Scene.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <TextComponent.h>
using json = nlohmann::json;

constexpr int TILE_SIZE = 48;

void LevelBuilder::LoadLevelFromFile(const std::string& path, dae::Scene& scene)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Failed to open level file: " << path << '\n';
        return;
    }

    json levelJson;
    try
    {
        file >> levelJson;
    }
    catch (const json::parse_error& e)
    {
        std::cerr << "JSON parse error in " << path << ": " << e.what() << '\n';
        return;
    }

    int width = levelJson.value("width", 0);
    int height = levelJson.value("height", 0);

    std::vector<std::vector<bool>> dugOut(height, std::vector<bool>(width, false));

    std::string mode = levelJson.value("mode", "Single");

    // === Tunnels ===
    if (levelJson.contains("tunnels"))
    {
        for (const auto& pos : levelJson["tunnels"])
        {
            int col = pos[0];
            int row = pos[1];
            if (row < height && col < width)
                dugOut[row][col] = true;
        }
    }

    // === Emeralds ===
    if (levelJson.contains("emeralds"))
    {
        for (const auto& pos : levelJson["emeralds"])
        {
            SpawnEmerald(scene, { pos[0], pos[1] });
            dugOut[pos[1]][pos[0]] = true;
        }
    }

    // === Gold Bags ===
    if (levelJson.contains("goldBags"))
    {
        for (const auto& pos : levelJson["goldBags"])
        {
            SpawnGoldBag(scene, { pos[0], pos[1] });
            dugOut[pos[1]][pos[0]] = true;
        }
    }

    // === Player ===
    if (levelJson.contains("playerSpawns") && !levelJson["playerSpawns"].empty())
    {
        int col = levelJson["playerSpawns"][0][0];
        int row = levelJson["playerSpawns"][0][1];
        SpawnPlayer(scene, { col, row });
        dugOut[row][col] = true;
    }

    // === Enemies ===
    if (levelJson.contains("enemies"))
    {
        for (const auto& enemy : levelJson["enemies"])
        {
            std::string type = enemy.value("type", "Nobbin");
            auto pos = enemy["pos"];
            int col = pos[0];
            int row = pos[1];

            if (type == "Nobbin")
                SpawnNobbin(scene, { col, row });
            else if (type == "Hobbin")
                SpawnHobbin(scene, { col, row });

            dugOut[row][col] = true;
        }
    }

    // === Dirt Generation ===
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            //Spawns dirt everywhere except where has been dug out
            if (!dugOut[row][col])
            {
                SpawnDirt(scene, { col, row });
            }
        }
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
void LevelBuilder::SpawnDirt(dae::Scene& scene, std::pair<int, int> gridPos)
{
    auto go = CreateBasicGO(scene, gridPos, "dirt.png");
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
