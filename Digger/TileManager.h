#pragma once
#include <vector>
#include <memory>
#include <string>
#include "TileComponent.h"
#include "Observer.h"
#include "Singleton.h"

class TileManager final : public dae::Observer, public dae::Singleton<TileManager>
{
public:
    TileManager() = default;
    ~TileManager() override = default;

    void Initialize(int width, int height);

    void RegisterTile(int row, int col, std::shared_ptr<TileComponent> tile);
    std::shared_ptr<TileComponent> GetTile(int row, int col) const;

    const std::vector<std::vector<std::shared_ptr<TileComponent>>>& GetTileMap() const { return m_TileMap; }

    int GetWidth() const { return static_cast<int>(m_TileMap.empty() ? 0 : m_TileMap[0].size()); }
    int GetHeight() const { return static_cast<int>(m_TileMap.size()); }

    void OnNotify(dae::Event event, dae::GameObject* pGameObject) override;

private:
    std::vector<std::vector<std::shared_ptr<TileComponent>>> m_TileMap;
};
