#include "TileManager.h"
#include <algorithm>
#include <iostream>

void TileManager::Initialize(int width, int height)
{
    // Tilemap
    m_TileMap.resize(height, std::vector<std::shared_ptr<TileComponent>>(width));
    
    // Enemies
    m_Enemies.assign(height, std::vector<std::vector<dae::GameObject*>>(width));
}

void TileManager::RegisterTile(int row, int col, std::shared_ptr<TileComponent> tile)
{
    if (row >= 0 && row < GetHeight() && col >= 0 && col < GetWidth())
    {
        m_TileMap[row][col] = tile;
    }
}

bool TileManager::IsValidTile(int row, int col) const
{
    return row >= 0 && col >= 0 &&
        row < static_cast<int>(m_TileMap.size()) &&
        col < static_cast<int>(m_TileMap[row].size());
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

    if (interactable->HasComponent<EmeraldComponent>())
		++m_TotalEmeralds;

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

void TileManager::RegisterEnemy(int row, int col, dae::GameObject* go)
{
    auto& v = m_Enemies[row][col];
    if (std::find(v.begin(), v.end(), go) == v.end())
        v.push_back(go);
}

void TileManager::RemoveEnemy(int row, int col, dae::GameObject* go)
{
    // bounds checking to avoid crash
    if (go == nullptr)
        return;

    if (row < 0 || row >= static_cast<int>(m_Enemies.size()))
        return;

    if (col < 0 || col >= static_cast<int>(m_Enemies[row].size()))
        return;

    auto& v = m_Enemies[row][col];
    v.erase(std::remove(v.begin(), v.end(), go), v.end());
}

const std::vector<dae::GameObject*>& TileManager::GetEnemiesAt(int row, int col) const
{
    static const std::vector<dae::GameObject*> emptyList;

    if (row < 0 || col < 0 || row >= static_cast<int>(m_Enemies.size()) || col >= static_cast<int>(m_Enemies[row].size()))
    {
        return emptyList;
    }

    return m_Enemies[row][col];
}
std::vector<dae::GameObject*> TileManager::GetAllEnemies() const
{
    std::vector<dae::GameObject*> allEnemies;

    for (const auto& row : m_Enemies)
    {
        for (const auto& col : row)
        {
            for (auto* enemy : col)
            {
                if (enemy) allEnemies.push_back(enemy);
            }
        }
    }

    return allEnemies;
}
void TileManager::OnNotify(dae::Event e, dae::GameObject* go)
{
    switch (e)
    {
    case dae::Event::PlayerDied:
        std::cout << "[TileManager] Event: PlayerDied\n";
        break;
    case dae::Event::PlayerDugTile:
        std::cout << "[TileManager] Event: PlayerDugTile\n";
        break;
    case dae::Event::TileDug:
        std::cout << "[TileManager] Event: TileDug\n";
        break;
    case dae::Event::TileContainsEmerald:
        std::cout << "[TileManager] Event: TileContainsEmerald\n";
        break;
    case dae::Event::TileContainsGoldBag:
        std::cout << "[TileManager] Event: TileContainsGoldBag\n";
        break;
    case dae::Event::GoldBagDropped:
        std::cout << "[TileManager] Event: GoldBagDropped\n";
        break;
    case dae::Event::GoldCollected:
        std::cout << "[TileManager] Event: GoldCollected\n";
        break;
    case dae::Event::EmeraldCollected:
        ++m_CollectedEmeralds;
        break;
    case dae::Event::PlayerCollected8Emeralds:
        std::cout << "[TileManager] Event: PlayerCollected8Emeralds\n";
        break;
    case dae::Event::PlayerCollectedAllEmeralds:
        std::cout << "[TileManager] Event: PlayerCollectedAllEmeralds\n";
        break;
    case dae::Event::LevelCompleted:
        std::cout << "[TileManager] Event: LevelCompleted\n";
        break;
    case dae::Event::EnemyKilled:
        std::cout << "[TileManager] Event: EnemyKilled\n";
        break;
    default:
        break;
    }
    CheckLevelCompletion(go);
}

//TODO: not sure about this implementation in TileManager? Move to LevelManager? 
void TileManager::CheckLevelCompletion(dae::GameObject* go)
{
    bool allEmeraldsCollected = m_CollectedEmeralds >= m_TotalEmeralds;
    bool allEnemiesGone = true;

    for (const auto& row : m_Enemies)
    {
        for (const auto& cell : row)
        {
            if (!cell.empty())
            {
                allEnemiesGone = false;
                break;
            }
        }
        if (!allEnemiesGone)
            break;
    }

	// If all emeralds are collected or all enemies are gone, load the next level
    if (allEmeraldsCollected || allEnemiesGone)
    {
        LevelManager::GetInstance().LoadNextLevel();
    }
}

