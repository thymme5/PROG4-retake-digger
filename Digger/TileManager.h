#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "TileComponent.h"
#include "Observer.h"
#include "Singleton.h"
#include "GameObject.h"
#include "EmeraldComponent.h"
#include "SubjectComponent.h"
#include "LevelManager.h"

class TileManager final : public dae::Observer, public dae::Singleton<TileManager>
{
public:
    TileManager() = default;
    ~TileManager() override = default;

    void Initialize(int width, int height);

    // register/get tiles
    void RegisterTile(int row, int col, std::shared_ptr<TileComponent> tile);
    std::shared_ptr<TileComponent> GetTile(int row, int col) const;
    
    // boolean to check if tile is valid
    bool IsValidTile(int row, int col) const;

    // interactable management
    void RegisterInteractable(int row, int col, dae::GameObject* interactable);
    std::vector<dae::GameObject*> GetInteractablesAt(int row, int col) const;
    void RemoveInteractable(int row, int col, dae::GameObject* interactable);

    // enemy management
    void RegisterEnemy(int row, int col, dae::GameObject* go);
    void RemoveEnemy(int row, int col, dae::GameObject* go);
    const std::vector<dae::GameObject*>& GetEnemiesAt(int row, int col) const;

    //Tile map
    const std::vector<std::vector<std::shared_ptr<TileComponent>>>& GetTileMap() const { return m_TileMap; }

    int GetWidth() const { return static_cast<int>(m_TileMap.empty() ? 0 : m_TileMap[0].size()); }
    int GetHeight() const { return static_cast<int>(m_TileMap.size()); }

    void OnNotify(dae::Event event, dae::GameObject* pGameObject) override;

private:
    std::vector<std::vector<std::shared_ptr<TileComponent>>> m_TileMap;
    std::unordered_map<int, std::unordered_map<int, std::vector<dae::GameObject*>>> m_Interactables;
    std::vector<std::vector<std::vector<dae::GameObject*>>> m_Enemies;

    int m_TotalEmeralds = 0;
    int m_CollectedEmeralds = 0;

    void CheckLevelCompletion(dae::GameObject* go);
};
