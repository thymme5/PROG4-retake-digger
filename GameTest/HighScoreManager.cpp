#include "HighscoreManager.h"
#include <fstream>
#include <algorithm>
#include <nlohmann/json.hpp>


const std::vector<HighscoreEntry>& HighscoreManager::GetHighscores() const
{
    return m_Highscores;
}

void HighscoreManager::AddHighscore(const HighscoreEntry& entry)
{
    m_LastAddedEntry = entry; // last score

    m_Highscores.push_back(entry);

    std::sort(m_Highscores.begin(), m_Highscores.end(), [](const auto& a, const auto& b) {
        return a.score > b.score;
        });

    if (m_Highscores.size() > 5)
        m_Highscores.resize(5);
}


void HighscoreManager::LoadHighscores()
{
    m_Highscores.clear();

    std::ifstream file(m_FilePath);
    if (!file.is_open()) return;

    nlohmann::json data;
    file >> data;

    for (const auto& entry : data)
    {
        if (entry.contains("initials") && entry.contains("score"))
        {
            m_Highscores.emplace_back(HighscoreEntry{ entry["initials"], entry["score"] });
        }
    }
}

void HighscoreManager::SaveHighscores() const
{
    nlohmann::json data;
    for (const auto& entry : m_Highscores)
    {
        data.push_back({
            { "initials", entry.initials },
            { "score", entry.score }
            });
    }

    std::ofstream file(m_FilePath);
    if (file.is_open())
        file << data.dump(4);
}
HighscoreEntry& HighscoreManager::GetLastAddedEntry()
{ 
    return m_LastAddedEntry;
}