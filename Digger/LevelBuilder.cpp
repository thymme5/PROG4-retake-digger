#include "LevelBuilder.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <TextComponent.h>
#include "TileManager.h"
#include "TileComponent.h"
#include "GameObject.h"
#include "UIComponent.h"
#include "LevelManager.h"
#include "ResourceManager.h"
#include "PlayerComponent.h"
#include "AliveState.h"
#include "EmeraldComponent.h"
#include "GoldBagComponent.h"
#include "EnemyComponent.h"
#include "NobbinControlState.h"
#include "NobbinState.h"
#include "ScoreManager.h"
#include "GameDirectorComponent.h"
using json = nlohmann::json;

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
    const std::string mode = levelJson.value("mode", "Solo");

    if (width <= 0 || height <= 0)
    {
        std::cerr << "Invalid level size\n";
        return;
    }

    auto directorGO = std::make_shared<dae::GameObject>();
    directorGO->AddComponent<GameDirectorComponent>(*directorGO);
    scene.Add(directorGO);

    TileManager::GetInstance().Initialize(width, height);

    // === UI ===
    auto hudGO = std::make_shared<dae::GameObject>();
    hudGO->SetLocalPosition(8.f, 8.f);
    hudGO->AddComponent<dae::TextComponent>(*hudGO, "SCORE 000000  LIVES 4\nLEVEL 1", dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));
    auto* ui = hudGO->AddComponent<UIComponent>(*hudGO);
    scene.Add(hudGO);

    // === Tiles ===
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            auto tileGO = std::make_shared<dae::GameObject>();
            tileGO->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);

            auto tileComp = tileGO->AddComponent<TileComponent>(*tileGO, row, col);
            tileGO->AddComponent<dae::TextureComponent>(*tileGO, "dirt.png", 1.f, 0);

            TileManager::GetInstance().RegisterTile(row, col, tileComp->GetOwner());
            scene.Add(tileGO);
        }
    }

    // === Tunnels ===
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

    // === Emeralds ===
    if (levelJson.contains("emeralds"))
    {
        for (const auto& pos : levelJson["emeralds"])
        {
            int col = pos[0];
            int row = pos[1];

            if (auto tile = TileManager::GetInstance().GetTile(row, col))
            {
                tile->SetHasEmerald(true);

                auto emeraldGO = std::make_shared<dae::GameObject>();
                emeraldGO->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);
                emeraldGO->AddComponent<dae::TextureComponent>(*emeraldGO, "Emerald.png", 1.f, 1);
                emeraldGO->AddComponent<EmeraldComponent>(*emeraldGO, row, col);

                TileManager::GetInstance().RegisterInteractable(row, col, emeraldGO.get());
                scene.Add(emeraldGO);
            }
        }
    }

    // === Gold Bags ===
    if (levelJson.contains("goldBags"))
    {
        for (const auto& pos : levelJson["goldBags"])
        {
            int col = pos[0];
            int row = pos[1];

            if (auto tile = TileManager::GetInstance().GetTile(row, col))
            {
                auto goldBagGO = std::make_shared<dae::GameObject>();
                goldBagGO->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);
                goldBagGO->AddComponent<dae::TextureComponent>(*goldBagGO, "goldbag.png", 1.f, 1);
                goldBagGO->AddComponent<GoldBagComponent>(*goldBagGO, row, col);
                goldBagGO->AddComponent<dae::SubjectComponent>(*goldBagGO);

                if (auto* subj = goldBagGO->GetComponent<dae::SubjectComponent>())
                    subj->AddObserver(&ScoreManager::GetInstance());

                TileManager::GetInstance().RegisterInteractable(row, col, goldBagGO.get());
                scene.Add(goldBagGO);
            }
        }
    }

    // === Player Spawns ===
    const auto playerSpawns = levelJson.value("playerSpawns", std::vector<std::vector<int>>{});
    SpawnPlayers(playerSpawns, scene, ui, mode);

    // === Enemies ===
    if (mode == "Versus")
    {
        if (playerSpawns.size() >= 2)
        {
            int col = playerSpawns[1][0];
            int row = playerSpawns[1][1];

            auto enemyGO = std::make_shared<dae::GameObject>();
            enemyGO->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);
            enemyGO->AddComponent<dae::TextureComponent>(*enemyGO, "nobbin.png", 1.f, 0);
            enemyGO->AddComponent<EnemyComponent>(*enemyGO, row, col, true); // player-controlled
            scene.Add(enemyGO);
        }
    }
    else if (levelJson.contains("enemies"))
    {
        for (const auto& pos : levelJson["enemies"])
        {
            if (!pos.is_array() || pos.size() < 2) continue;

            int col = pos[0];
            int row = pos[1];

            if (!TileManager::GetInstance().GetTile(row, col)) continue;

            auto enemyGO = std::make_shared<dae::GameObject>();
            enemyGO->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);
            enemyGO->AddComponent<dae::TextureComponent>(*enemyGO, "nobbin.png", 1.f, 0);
            enemyGO->AddComponent<EnemyComponent>(*enemyGO, row, col); // computer
            scene.Add(enemyGO);
        }
    }

}

void LevelBuilder::SpawnPlayers(const std::vector<std::vector<int>>& spawns, dae::Scene& scene, UIComponent* ui, const std::string& mode)
{
    size_t playerCount = (mode == "Coop") ? 2 : 1;

    for (size_t i = 0; i < playerCount && i < spawns.size(); ++i)
    {
        int col = spawns[i][0];
        int row = spawns[i][1];

        auto playerGO = std::make_shared<dae::GameObject>();
        playerGO->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);

        std::string texture = (i == 0) ? "digger.png" : "digger2.png";
        playerGO->AddComponent<dae::TextureComponent>(*playerGO, texture, 1.f, 0);
        playerGO->AddComponent<dae::SubjectComponent>(*playerGO);
        auto* playerComp = playerGO->AddComponent<PlayerComponent>(*playerGO, row, col);
        playerComp->SetState(std::make_unique<AliveState>());

        scene.Add(playerGO);

        if (auto* subj = playerGO->GetComponent<dae::SubjectComponent>())
        {
            subj->AddObserver(&ScoreManager::GetInstance());
            subj->AddObserver(&TileManager::GetInstance());
        }
    }
}
