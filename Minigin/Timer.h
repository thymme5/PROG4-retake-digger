#pragma once
#include <chrono>

class Timer
{
public:
    static void Init()
    {
        m_LastTime = std::chrono::high_resolution_clock::now();
    }

    static void Update()
    {
        auto now = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = now - m_LastTime;
        m_DeltaTime = elapsed.count();
        m_LastTime = now;
    }

    static float GetDeltaTime() { return m_DeltaTime; }

private:
    static inline std::chrono::high_resolution_clock::time_point m_LastTime;
    static inline float m_DeltaTime{ 0.0f };
};
