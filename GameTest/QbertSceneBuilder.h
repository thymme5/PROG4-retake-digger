#pragma once
#include <memory>
#include <iostream>
#include "Scene.h"
#include "GameplayManagerComponent.h"
#include "GameModeManager.h"
#include "BackToMenuCommand.h"
#include "FinishRoundCommand.h"
#include "EdgeEnemyComponent.h"

class QbertSceneBuilder
{
public:
    
    //menus
    static void BuildMainMenu(dae::Scene& scene, const std::shared_ptr<dae::Font>& font);
    static void BuildHighScoreScene(dae::Scene& scene);
    static void BuildGameOverScene(dae::Scene& scene);
    static void BuildVictoryScene(dae::Scene& scene);

    static void BuildQbertBaseScene(dae::Scene& scene, const std::string& levelPath);
    static void CreateQbertPlayer(const std::shared_ptr<TileComponent>& startTile, dae::Scene& scene, bool isSecondPlayer);

    static std::shared_ptr<dae::GameObject> SpawnCoily(const std::shared_ptr<TileComponent>& startTile, const std::shared_ptr<dae::GameObject>& qbert, bool isPlayerControlled);

    //different gamemodes
    static void BuildSinglePlayerScene(dae::Scene& scene, const std::string& levelPath);
    static void BuildCoopScene(dae::Scene& scene, const std::string& levelPath);
    static void BuildVersusScene(dae::Scene& scene, const std::string& levelPath);
};
