#pragma once
#include "Command.h"
#include "SoundService.h"

class ToggleMuteCommand final : public dae::Command
{
public:
    void Execute() override
    {
        auto* soundService = dae::ServiceLocator::GetSoundService();
        if (!soundService) return;

        const bool currentlyMuted = soundService->IsMuted();
        soundService->SetMuted(!currentlyMuted);

		std::cout << "[ToggleMuteCommand] sound" << (currentlyMuted ? " off" : " on") << ".\n";
    }
};
