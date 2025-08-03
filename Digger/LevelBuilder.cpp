#include "LevelBuilder.h"
#include "TextureComponent.h"
#include "Scene.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <TextComponent.h>
#include "TileComponent.h"
#include "TileManager.h"
#include "SubjectComponent.h"
#include "PlayerComponent.h"
#include "AliveState.h"
#include "EmeraldComponent.h"
#include "GoldBagComponent.h"


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
    try { file >> levelJson; }
    catch (const json::parse_error& e)
    {
        std::cerr << "JSON parse error: " << e.what() << '\n';
        return;
    }

    const int width = levelJson.value("width", 0);
    const int height = levelJson.value("height", 0);

    if (width <= 0 || height <= 0)
    {
        std::cerr << "Invalid level size\n";
        return;
    }

    TileManager::GetInstance().Initialize(width, height);

    // === TILE GRID ===
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            auto tileGO = std::make_shared<dae::GameObject>();
            tileGO->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);

            auto tileComp = tileGO->AddComponent<TileComponent>(*tileGO, row, col);
            tileComp->SetDug(false); // Default: not dug == has dirt

            tileGO->AddComponent<dae::TextureComponent>(*tileGO, "dirt.png", 1.f, 0);

            TileManager::GetInstance().RegisterTile(row, col, std::shared_ptr<TileComponent>(tileComp, [](TileComponent*) {}));

            scene.Add(tileGO);
        }
    }

    // === TUNNELS ===
    if (levelJson.contains("tunnels"))
    {
        for (const auto& pos : levelJson["tunnels"])
        {
            int col = pos[0];
            int row = pos[1];

            if (auto tile = TileManager::GetInstance().GetTile(row, col))
            {
                tile->SetDug(true);
                
                tile->GetGameObject()->GetComponent<dae::TextureComponent>()->SetVisible(false);
            }
        }
    }

    // === EMERALDS ===
    if (levelJson.contains("emeralds"))
    {
        for (const auto& pos : levelJson["emeralds"])
        {
            int col = pos[0];
            int row = pos[1];

            if (auto tile = TileManager::GetInstance().GetTile(row, col))
            {
                // TODO: remove this line it's dumb and unnecessary
                tile->SetHasEmerald(true);

                // Create Emerald GameObject
                auto emeraldGO = std::make_shared<dae::GameObject>();
                emeraldGO->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);
                emeraldGO->AddComponent<dae::TextureComponent>(*emeraldGO, "Emerald.png", 1.f, 1);
                emeraldGO->AddComponent<EmeraldComponent>(*emeraldGO, row, col);

                // Register as an interactable in the TileManager
                TileManager::GetInstance().RegisterInteractable(row, col, emeraldGO.get());

                // Add to scene
                scene.Add(emeraldGO);
            }
        }
    }

    // === GOLD BAGS ===
    if (levelJson.contains("goldBags"))
    {
        for (const auto& pos : levelJson["goldBags"])
        {
            int col = pos[0];
            int row = pos[1];

            // If tile exists
            if (auto tile = TileManager::GetInstance().GetTile(row, col))
            {
                // Create GoldBag GameObject
                auto goldBagGO = std::make_shared<dae::GameObject>();
                goldBagGO->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);

                std::cout << "GoldBag spawned at (" << row << ", " << col << ")\n";

                // Add components
                goldBagGO->AddComponent<dae::TextureComponent>(*goldBagGO, "goldbag.png", 1.f, 1);
                goldBagGO->AddComponent<GoldBagComponent>(*goldBagGO, row, col);

                // Register as interactable
                TileManager::GetInstance().RegisterInteractable(row, col, goldBagGO.get());

                // Add to scene
                scene.Add(goldBagGO);
            }
        }
    }

    // === PLAYER SPAWN ===
    if (levelJson.contains("playerSpawns") && !levelJson["playerSpawns"].empty())
    {
        int col = levelJson["playerSpawns"][0][0];
        int row = levelJson["playerSpawns"][0][1];

        auto playerGO = std::make_shared<dae::GameObject>();
        playerGO->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);
        playerGO->AddComponent<dae::TextureComponent>(*playerGO, "digger.png", 1.f, 0);
		playerGO->AddComponent<dae::SubjectComponent>(*playerGO);
        playerGO->AddComponent<PlayerComponent>(*playerGO, row, col)->SetState(std::make_unique<AliveState>());

        scene.Add(playerGO);
    }

    // === ENEMIES ===
    if (levelJson.contains("enemies"))
    {
        for (const auto& enemy : levelJson["enemies"])
        {
            std::string type = enemy.value("type", "Nobbin");
            auto pos = enemy["pos"];
            int col = pos[0];
            int row = pos[1];

            auto enemyGO = std::make_shared<dae::GameObject>();
            enemyGO->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);

            if (type == "Nobbin")
                enemyGO->AddComponent<dae::TextureComponent>(*enemyGO, "nobbin.png", 1.f, 0);
            else if (type == "Hobbin")
                enemyGO->AddComponent<dae::TextureComponent>(*enemyGO, "hobbin.png", 1.f, 0);

            scene.Add(enemyGO);
        }
    }
}
