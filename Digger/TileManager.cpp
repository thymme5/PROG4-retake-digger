#include "TileManager.h"
#include "TileComponent.h"
#include <iostream>

void TileManager::RegisterTile(int row, int col, std::shared_ptr<TileComponent> tile)
{
    m_TileMap[row][col] = tile;
}

std::shared_ptr<TileComponent> TileManager::GetTile(int row, int col) const
{
    if (row < 0 || row >= static_cast<int>(m_TileMap.size()))
        return nullptr;
    if (col < 0 || col >= static_cast<int>(m_TileMap[row].size()))
        return nullptr;

    return m_TileMap[row][col];
}

void TileManager::OnNotify(dae::Event event, dae::GameObject* gameObject)
{

}
void TileManager::Initialize(int width, int height)
{
    m_TileMap.resize(height, std::vector<std::shared_ptr<TileComponent>>(width));
}
