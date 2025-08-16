#pragma once
#include <string>
#include <unordered_map>
#include "SoundService.h"
#include <iostream>

enum class SoundID
{
    BackgroundMusic,
    DiggerGameOver,
};


class DiggerSoundLibrary
{
public:
    static void LoadAllSounds();
    static void Play(SoundID id);
    static void Stop(SoundID id);

    static bool IsPlaying(SoundID id);
private:
    static void Register(SoundID id, const std::string& path);
    static std::unordered_map<SoundID, std::string> m_SoundMap;
};

