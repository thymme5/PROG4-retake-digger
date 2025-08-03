#include "TileManager.h"
#include <algorithm>
#include <iostream>

void TileManager::Initialize(int width, int height)
{
    m_TileMap.resize(height, std::vector<std::shared_ptr<TileComponent>>(width));
}

void TileManager::RegisterTile(int row, int col, std::shared_ptr<TileComponent> tile)
{
    if (row >= 0 && row < GetHeight() && col >= 0 && col < GetWidth())
    {
        m_TileMap[row][col] = tile;
    }
}

std::shared_ptr<TileComponent> TileManager::GetTile(int row, int col) const
{
    if (row >= 0 && row < GetHeight() && col >= 0 && col < GetWidth())
    {
        return m_TileMap[row][col];
    }
    return nullptr;
}

void TileManager::RegisterInteractable(int row, int col, dae::GameObject* interactable)
{
    if (interactable == nullptr)
        return;

    m_Interactables[row][col].push_back(interactable);
}

std::vector<dae::GameObject*> TileManager::GetInteractablesAt(int row, int col) const
{
    auto rowIt = m_Interactables.find(row);
    if (rowIt != m_Interactables.end())
    {
        auto colIt = rowIt->second.find(col);
        if (colIt != rowIt->second.end())
        {
            return colIt->second;
        }
    }
    return {};
}

void TileManager::RemoveInteractable(int row, int col, dae::GameObject* interactable)
{
    if (interactable == nullptr)
        return;

    auto rowIt = m_Interactables.find(row);
    if (rowIt != m_Interactables.end())
    {
        auto& colMap = rowIt->second;
        auto colIt = colMap.find(col);
        if (colIt != colMap.end())
        {
            auto& vec = colIt->second;
            vec.erase(std::remove(vec.begin(), vec.end(), interactable), vec.end());

            if (vec.empty())
            {
                colMap.erase(colIt);
                if (colMap.empty())
                {
                    m_Interactables.erase(rowIt);
                }
            }
        }
    }
}

void TileManager::OnNotify(dae::Event, dae::GameObject*)
{

}
