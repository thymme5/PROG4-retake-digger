#pragma once
#include <string>
#include <unordered_map>
#include "SoundService.h"
#include <iostream>

enum class SoundID
{
    QbertJump,
    CoilySnakeJump,
    CoilyEggJump,
    DiscWarp,
    RoundComplete,
    ChangeSelection,
    ClearDisks,
    CoilyFall,
    DemoSound,
    DiskLand,
    DiskLift,
    LevelScreenTune,
    OtherFoesJump,
    QbertFall,
    QbertHit,
    SlickSamCaught,
    Swearing
};

// felt like my old implementation of sound was a bit repetitive and error prone, 
// so I'm writing a small "sound library" to make playing sounds easier and make the main less cluttered

class QbertSoundLibrary
{
public:
    static void LoadAllSounds(); //loads all sounds in the main function
    static void Play(SoundID id); //plays whatever sound your heart desires 
    static bool IsPlaying(SoundID id); //checks if song is playing or not
private:
    static void Register(SoundID id, const std::string& path);
    static std::unordered_map<SoundID, std::string> m_SoundMap;
};

