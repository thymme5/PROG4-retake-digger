#pragma once
#include "Observer.h"
#include "Singleton.h"

namespace dae { class SubjectComponent; }

class ScoreManager final : public dae::Singleton<ScoreManager>, public dae::Observer
{
public:
    void ResetForNewGame()
    {
        m_score = 0;
        m_lives = 4;
        m_nextLifeScore = 20000;
    }

    void Set(int points) { m_score = points; }
    int  GetLives() const { return m_lives; }

    int  Get() const { return m_score; }

    void Add(int points) { m_score += points; }
    void AddNobbin() { m_score += kNobbin; }
    void AddHobbin() { m_score += kHobbin; }
    void AddEmerald() { m_score += kEmerald; }
    void AddGoldNugget() { m_score += kGoldNugget; }
    void AddEightEmeraldStreakBonus() { m_score += kEightEmeraldsInARow; }

    void GainLife(int n = 1) { m_lives += n; }
    void LoseLife(int n = 1) { m_lives = (m_lives > n) ? (m_lives - n) : 0; }

    void OnNotify(dae::Event event, dae::GameObject* sender) override;

    void Subscribe(dae::SubjectComponent& subject);
    void Unsubscribe(dae::SubjectComponent& subject);

    ~ScoreManager() override = default;

private:
    friend class dae::Singleton<ScoreManager>;
    ScoreManager() = default;

    void GrantExtraLivesIfNeeded()
    {
        while (m_score >= m_nextLifeScore)
        {
            ++m_lives;
            m_nextLifeScore += 20000;
        }
    }

    int m_score{ 0 };
    int m_lives{ 4 };
    int m_nextLifeScore{ 20000 };

    // Points table
    static constexpr int kNobbin = 250;
    static constexpr int kHobbin = 250;
    static constexpr int kEmerald = 25;
    static constexpr int kGoldNugget = 500;
    static constexpr int kEightEmeraldsInARow = 250;
};
