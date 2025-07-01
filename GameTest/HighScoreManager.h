#pragma once
#include <vector>
#include <string>
#include "Singleton.h"

struct HighscoreEntry
{
    std::string initials;
    int score;
};

class HighscoreManager final : public dae::Singleton<HighscoreManager>
{
public:
    const std::vector<HighscoreEntry>& GetHighscores() const;
    void AddHighscore(const HighscoreEntry& entry);
    void LoadHighscores();
    void SaveHighscores() const;
    HighscoreEntry& GetLastAddedEntry();

private:
    friend class dae::Singleton<HighscoreManager>; 

    HighscoreManager() = default;
    ~HighscoreManager() = default;

    std::vector<HighscoreEntry> m_Highscores;
    const std::string m_FilePath = "highscores.json";
    HighscoreEntry m_LastAddedEntry{};
};
