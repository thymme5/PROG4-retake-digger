#pragma once
#include "GameMode.h"
#include <string>

class PostGameEntryMode final : public GameMode
{
public:
    PostGameEntryMode() = default;

    void Enter() override;
    void Exit() override;
    void Update() override;

private:
    std::string m_SceneName{ "PostGameEntryScene" };
};
