#include "QbertSceneBuilder.h"

#include <memory>
#include <iostream>

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "SoundService.h"

#include "GameplayManagerComponent.h"
#include "QbertMoveComponent.h"
#include "CoilyComponent.h"
#include "EggState.h"
#include "TileComponent.h"
#include "LevelBuilder.h"
#include "GameUIComponent.h"
#include "TextComponent.h"
#include "TextureComponent.h"
#include "FPScounterComponent.h"
#include "ToggleMuteCommand.h"
#include "MoveCommand.h"
#include "FinishRoundCommand.h"
#include "QbertSoundLibrary.h"
#include "GameModeManager.h"
#include "MainMenuUIComponent.h"
#include "MoveMenuArrow.h"
#include "EnterGameMode.h"
#include "InputBindingHelper.h"
#include "HighScoreManager.h"

void QbertSceneBuilder::CreateQbertPlayer(const std::shared_ptr<TileComponent>& startTile, dae::Scene& scene, bool isSecondPlayer)
{
    auto qbert = std::make_shared<dae::GameObject>();
    qbert->AddComponent<dae::TextureComponent>(*qbert, isSecondPlayer ? "Qbert P2 Spritesheet.png" : "Qbert P1 Spritesheet.png", 2.f, 4);

    auto* moveComp = qbert->AddComponent<QbertMoveComponent>(*qbert);
    moveComp->SetTileMap(LevelBuilder::GetTileComponentMap());
    moveComp->SetCurrentTile(startTile);

    // === Create swear bubble as child ===
    auto swear = std::make_shared<dae::GameObject>();
    auto* textureComponent = swear->AddComponent<dae::TextureComponent>(*swear, "Qbert Curses.png", 2.f, 1);
    textureComponent->SetVisible(false);
    swear->SetParent(qbert.get());
    swear->SetPosition(-5.f, -30.f);

    scene.Add(qbert);
    scene.Add(swear);
}

std::shared_ptr<dae::GameObject> QbertSceneBuilder::SpawnCoily(const std::shared_ptr<TileComponent>& startTile, const std::shared_ptr<dae::GameObject>& qbert, bool isPlayerControlled)
{
    auto coily = std::make_shared<dae::GameObject>();

    constexpr float coilyScale{ 2.f };
    constexpr int coilyFrames{ 9 };

    coily->AddComponent<dae::TextureComponent>(*coily, "Coily Spritesheet.png", coilyScale, coilyFrames);

    auto* coilyComponent = coily->AddComponent<CoilyComponent>(*coily);
    coilyComponent->SetTileMap(LevelBuilder::GetTileComponentMap());
    coilyComponent->SetState(std::make_unique<EggState>());
    coilyComponent->SetCurrentTile(startTile);
    coilyComponent->SetQbert(qbert);
    coilyComponent->SetPlayerControlled(isPlayerControlled);

    return coily;
}

void QbertSceneBuilder::BuildGameOverScene(dae::Scene& scene)
{
    // === load texture ===
    auto gameOverTexture = dae::ResourceManager::GetInstance().LoadTexture("Game Over Title.png");

    // === create GO with texture ===
    auto titleGO = std::make_shared<dae::GameObject>();
    titleGO->AddComponent<dae::TextureComponent>(*titleGO, "Game Over Title.png");

    // center dumb shit to screen
    const auto screenWidth = dae::Renderer::GetInstance().GetWindowSize().x;
    const auto screenHeight = dae::Renderer::GetInstance().GetWindowSize().y;
    const auto textureSize = gameOverTexture->GetSize();

    const float x = (screenWidth - textureSize.x) / 2.0f;
    const float y = (screenHeight - textureSize.y) / 2.0f;

    titleGO->SetPosition(x, y);
    scene.Add(titleGO);

    // === input binding: Press ESC or B to return to menu ===
    auto& inputManager = dae::InputManager::GetInstance();
    auto backCommand = std::make_shared<EnterGameMode>(3);

    inputManager.BindCommand(SDLK_ESCAPE, KeyState::Down, backCommand);
    inputManager.BindCommand(0, GamepadButton::B, KeyState::Down, backCommand);
}
void QbertSceneBuilder::BuildMainMenu(dae::Scene& scene, const std::shared_ptr<dae::Font>& font)
{
    auto go = std::make_shared<dae::GameObject>();
    auto UIcomponent = go->AddComponent<MainMenuUIComponent>(*go);
    scene.Add(go);

    const float baseY = 300.0f;
    const float spacing = 50.0f;
    const auto windowWidth = dae::Renderer::GetInstance().GetWindowSize().x;

    std::vector<std::string> options = { "Solo mode", "Co-op Mode", "Versus mode", "High score"};

    float arrowX{};
    float arrowY{};

    for (size_t i = 0; i < options.size(); ++i)
    {
        auto optionGO = std::make_shared<dae::GameObject>();
        auto* text = optionGO->AddComponent<dae::TextComponent>(*optionGO, options[i], font);

        // force texture creation before getting size so that it works
        text->SetText(options[i]);
        text->Update();

        auto width = text->GetTextureSize().x;
        float x = (windowWidth - width) / 2.0f;
        float y = baseY + i * spacing;

        optionGO->SetPosition(x, y);
        scene.Add(optionGO);

        if (i == 0)
        {
            arrowX = x - 30.f; 
            arrowY = y;
        }
    }

    // === Selection Arrow ===
    auto arrowGO = std::make_shared<dae::GameObject>();
    arrowGO->AddComponent<dae::TextureComponent>(*arrowGO, "Selection Arrow.png", 2.f);
    arrowGO->SetPosition(arrowX, arrowY);
    UIcomponent->SetArrow(arrowGO);
    scene.Add(arrowGO);

    // === Input bindings ===
    auto& inputManager = dae::InputManager::GetInstance();

    auto moveArrowUp = std::make_shared<MoveMenuArrow>(UIcomponent, -1.f);
    inputManager.BindCommand(SDLK_UP, KeyState::Down, moveArrowUp);
    inputManager.BindCommand(0, GamepadButton::DPadUp, KeyState::Down, moveArrowUp);

    auto moveArrowDown = std::make_shared<MoveMenuArrow>(UIcomponent, 1.f);
    inputManager.BindCommand(SDLK_DOWN, KeyState::Down, moveArrowDown);
    inputManager.BindCommand(0, GamepadButton::DPadDown, KeyState::Down, moveArrowDown);

    auto confirmCommand = std::make_shared<EnterGameMode>(UIcomponent);
    inputManager.BindCommand(SDLK_RETURN, KeyState::Down, confirmCommand);
    inputManager.BindCommand(0, GamepadButton::A, KeyState::Down, confirmCommand);
}
void QbertSceneBuilder::BuildHighScoreScene(dae::Scene& scene)
{
    const auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    const auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
    const auto windowWidth = dae::Renderer::GetInstance().GetWindowSize().x;

    // === Title ===
    auto titleGO = std::make_shared<dae::GameObject>();
    auto* titleText = titleGO->AddComponent<dae::TextComponent>(*titleGO, "HIGH SCORES", font);
    titleText->Update();
    float x = (windowWidth - static_cast<float>(titleText->GetTextureSize().x)) / 2.f;
    titleGO->SetPosition(x, 100.f);
    scene.Add(titleGO);
    

    // === load and display scores ===
    HighscoreManager::GetInstance().LoadHighscores();
    const auto& scores = HighscoreManager::GetInstance().GetHighscores();
    const float baseY = 180.0f;
    const float spacing = 40.0f;

    for (size_t i = 0; i < scores.size(); ++i)
    {
        std::stringstream ss;
        ss << (i + 1) << ". " << scores[i].initials << " - " << scores[i].score;

        auto scoreGO = std::make_shared<dae::GameObject>();
        auto* scoreText = scoreGO->AddComponent<dae::TextComponent>(*scoreGO, ss.str(), font);
        scoreText->Update();

        float width = static_cast<float>(scoreText->GetTextureSize().x);
        x = (windowWidth - width) / 2.f;
        float y = baseY + static_cast<float>(i) * spacing;

        scoreGO->SetPosition(x, y);
        scene.Add(scoreGO);
    }

    // === Hint text ===
    
    auto hintGO = std::make_shared<dae::GameObject>();
    auto* hintText = hintGO->AddComponent<dae::TextComponent>(*hintGO, "ESC OR B TO GO BACK", smallFont);
    hintText->Update();

    x = (windowWidth - static_cast<float>(hintText->GetTextureSize().x)) / 2.f;
    hintGO->SetPosition(x, 400.f);
    scene.Add(hintGO);
    

    // === Input ===
    auto& input = dae::InputManager::GetInstance();
    auto confirmCmd = std::make_shared<BackToMenuCommand>();
    input.BindCommand(SDLK_ESCAPE, KeyState::Down, confirmCmd);
    input.BindCommand(0, GamepadButton::B, KeyState::Down, confirmCmd);
}
void QbertSceneBuilder::BuildVictoryScene(dae::Scene& scene)
{
    const auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    const auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
    const auto windowWidth = dae::Renderer::GetInstance().GetWindowSize().x;

    // === Victory Title ===
    {
        auto titleGO = std::make_shared<dae::GameObject>();
        titleGO->AddComponent<dae::TextureComponent>(*titleGO, "Victory Title.png", 1.f);
        float titleWidth = static_cast<float>(titleGO->GetComponent<dae::TextureComponent>()->GetTexture()->GetSize().x);
        float x = (windowWidth - titleWidth) / 2.f;
        titleGO->SetPosition(x, 50.f);
        scene.Add(titleGO);
    }
    int finalScore = HighscoreManager::GetInstance().GetLastAddedEntry().score;


    // === Score Display ===
    {
        auto scoreGO = std::make_shared<dae::GameObject>();
        std::stringstream ss;
        ss << "YOUR SCORE: " << finalScore;
        auto* scoreText = scoreGO->AddComponent<dae::TextComponent>(*scoreGO, ss.str(), font);
        scoreText->Update();
        float x = (windowWidth - static_cast<float>(scoreText->GetTextureSize().x)) / 2.f;
        scoreGO->SetPosition(x, 220.f);
        scene.Add(scoreGO);
    }

    // === Continue Hint ===
    {
        auto hintGO = std::make_shared<dae::GameObject>();
        auto* hintText = hintGO->AddComponent<dae::TextComponent>(*hintGO, "ESC OR B TO RETURN TO MENU", smallFont);
        hintText->Update();
        float x = (windowWidth - static_cast<float>(hintText->GetTextureSize().x)) / 2.f;
        hintGO->SetPosition(x, 350.f);
        scene.Add(hintGO);
    }

    // === Input ===
    auto& input = dae::InputManager::GetInstance();
    auto confirmCmd = std::make_shared<BackToMenuCommand>();
    input.BindCommand(SDLK_ESCAPE, KeyState::Down, confirmCmd);
    input.BindCommand(0, GamepadButton::B, KeyState::Down, confirmCmd);
}
void QbertSceneBuilder::BuildQbertBaseScene(dae::Scene& scene, const std::string& levelPath)
{
    auto& inputManager = dae::InputManager::GetInstance();
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    // === Gameplay Manager ===
    auto controller = std::make_shared<dae::GameObject>();
    auto* manager = controller->AddComponent<GameplayManagerComponent>(*controller);
    manager->Init(scene, levelPath);
    manager->SetInstance(manager);
    scene.Add(controller);

    // === FPS counter ===
    auto fpsGO = std::make_shared<dae::GameObject>();
    fpsGO->AddComponent<dae::TextComponent>(*fpsGO, "0 FPS", font);
    fpsGO->AddComponent<dae::FPScounterComponent>(*fpsGO);
    scene.Add(fpsGO);

    // === Game UI ===
    auto uiGO = std::make_shared<dae::GameObject>();
    auto* gameUI = uiGO->AddComponent<dae::GameUIComponent>(*uiGO);
    uiGO->AddComponent<dae::TextComponent>(*uiGO, " ", dae::ResourceManager::GetInstance().LoadFont("minecraft.ttf", 18));
    uiGO->SetPosition(5, 50);
    scene.Add(uiGO);

    manager->SetGameUI(gameUI); 
    controller->AddObserver(gameUI);

    // === Round & Level UI ===
    auto roundGO = std::make_shared<dae::GameObject>();
    roundGO->AddComponent<dae::TextComponent>(*roundGO, "LEVEL: 1", font);
    roundGO->SetPosition(400, 80);
    scene.Add(roundGO);

    gameUI->SetRoundText(std::shared_ptr<dae::TextComponent>(roundGO->GetComponent<dae::TextComponent>(), [](dae::TextComponent*) {}));

    // === Mute toggle ===
    auto toggleMute = std::make_shared<ToggleMuteCommand>();
    inputManager.BindCommand(SDLK_F2, KeyState::Down, toggleMute);

    // === Skip round === 
    auto skipRound = std::make_shared<FinishRoundCommand>(manager);
    inputManager.BindCommand(SDLK_F4, KeyState::Down, skipRound);

}
void QbertSceneBuilder::BuildSinglePlayerScene(dae::Scene& scene, const std::string& levelPath)
{
    BuildQbertBaseScene(scene, levelPath);

    auto tileMap = LevelBuilder::GetTileMap();
    auto startTiles = LevelBuilder::GetStartTileIndices();
    auto tileGO = tileMap[startTiles[0].first][startTiles[0].second];
    auto tileComp = std::shared_ptr<TileComponent>(tileGO->GetComponent<TileComponent>(), [](TileComponent*) {});

    CreateQbertPlayer(tileComp, scene, false);

    auto qbert = scene.FindFirstObjectOfType<QbertMoveComponent>();

    const int startRow = 0;
    const int startCol = static_cast<int>(tileMap[startRow].size()) / 2;
    auto coilyTileGO = tileMap[startRow][startCol];
    auto coilyTile = std::shared_ptr<TileComponent>(coilyTileGO->GetComponent<TileComponent>(), [](TileComponent*) {});

    auto coily = SpawnCoily(coilyTile, qbert, false);
    scene.Add(coily);

    if (auto managerGO = scene.FindFirstObjectOfType<GameplayManagerComponent>())
    {
        auto* manager = managerGO->GetComponent<GameplayManagerComponent>();
        manager->SetQbert(qbert);
        if (auto* gameUI = manager->GetGameUI())
        {
            manager->SetQbert(qbert);
            manager->SetCoily(coily);

            qbert->AddObserver(gameUI);
            qbert->AddObserver(manager);

            coily->AddObserver(gameUI);
            coily->AddObserver(manager);
        }
    }

    //// === Spawn Ugg ===
    //{
    //    auto uggGO = std::make_shared<dae::GameObject>();

    //    uggGO->AddComponent<dae::TextureComponent>(*uggGO, "Ugg.png", 2.f, 0);
    //    auto uggComp = uggGO->AddComponent<EdgeEnemyComponent>(EdgeEnemyType::Ugg, *uggGO);
    //    uggComp->SetTileMap(LevelBuilder::GetTileComponentMap());

    //    scene.Add(uggGO);
    //}

    //// === Spawn Wrongway ===
    //{
    //    auto wrongwayGO = std::make_shared<dae::GameObject>();

    //    wrongwayGO->AddComponent<dae::TextureComponent>(*wrongwayGO, "Wrongway.png", 2.f, 0);
    //    auto wrongwayComp = wrongwayGO->AddComponent<EdgeEnemyComponent>(EdgeEnemyType::Wrongway, *wrongwayGO);
    //    wrongwayComp->SetTileMap(LevelBuilder::GetTileComponentMap());

    //    scene.Add(wrongwayGO);
    //}

    InputBindingHelper::BindPlayer1GamepadInputs(qbert.get());
    InputBindingHelper::BindPlayer1KeyboardInputs(qbert.get());
}

void QbertSceneBuilder::BuildCoopScene(dae::Scene& scene, const std::string& levelPath)
{
    BuildQbertBaseScene(scene, levelPath);

    auto tileMap = LevelBuilder::GetTileMap();
    auto startTiles = LevelBuilder::GetStartTileIndices();

    auto p1TileGO = tileMap[startTiles[0].first][startTiles[0].second];
    auto p1Tile = std::shared_ptr<TileComponent>(p1TileGO->GetComponent<TileComponent>(), [](TileComponent*) {});
    CreateQbertPlayer(p1Tile, scene, false);

    auto p2TileGO = tileMap[startTiles[1].first][startTiles[1].second];
    auto p2Tile = std::shared_ptr<TileComponent>(p2TileGO->GetComponent<TileComponent>(), [](TileComponent*) {});
    CreateQbertPlayer(p2Tile, scene, true);

    auto qbertObjects = scene.FindObjectsOfType<QbertMoveComponent>();
    if (qbertObjects.size() < 2)
        return;

    auto qbert1 = qbertObjects[0];
    auto qbert2 = qbertObjects[1];

    const int startRow = 0;
    const int startCol = static_cast<int>(tileMap[startRow].size()) / 2;
    auto coilyTileGO = tileMap[startRow][startCol];
    auto coilyTile = std::shared_ptr<TileComponent>(coilyTileGO->GetComponent<TileComponent>(), [](TileComponent*) {});

    auto coily = SpawnCoily(coilyTile, qbert1, false);
    scene.Add(coily);

    if (auto managerGO = scene.FindFirstObjectOfType<GameplayManagerComponent>())
    {
        auto* manager = managerGO->GetComponent<GameplayManagerComponent>();
        manager->SetQbert(qbert1);
        if (auto* gameUI = manager->GetGameUI())
        {
            manager->SetCoily(coily);

            qbert1->AddObserver(gameUI);
            qbert2->AddObserver(gameUI);

            coily->AddObserver(gameUI);
            coily->AddObserver(manager);
        }
    }

    InputBindingHelper::BindPlayer1KeyboardInputs(qbert1.get());
    InputBindingHelper::BindPlayer2GamepadInputs(qbert2.get());
}

void QbertSceneBuilder::BuildVersusScene(dae::Scene& scene, const std::string& levelPath)
{
    BuildQbertBaseScene(scene, levelPath);

    auto tileMap = LevelBuilder::GetTileMap();
    auto startTiles = LevelBuilder::GetStartTileIndices();

    auto qbertTileGO = tileMap[startTiles[0].first][startTiles[0].second];
    auto qbertTile = std::shared_ptr<TileComponent>(qbertTileGO->GetComponent<TileComponent>(), [](TileComponent*) {});
    CreateQbertPlayer(qbertTile, scene, false);

    auto qbert = scene.FindFirstObjectOfType<QbertMoveComponent>();
    if (!qbert)
        return;

    const int startRow = 0;
    const int startCol = static_cast<int>(tileMap[startRow].size()) / 2;
    auto coilyTileGO = tileMap[startRow][startCol];
    auto coilyTile = std::shared_ptr<TileComponent>(coilyTileGO->GetComponent<TileComponent>(), [](TileComponent*) {});

    auto coily = SpawnCoily(coilyTile, qbert, true);
    scene.Add(coily);

    if (auto managerGO = scene.FindFirstObjectOfType<GameplayManagerComponent>())
    {
        auto* manager = managerGO->GetComponent<GameplayManagerComponent>();
        manager->SetQbert(qbert);
        manager->SetCoily(coily);

        if (auto* gameUI = manager->GetGameUI())
        {
            qbert->AddObserver(gameUI);
            qbert->AddObserver(manager);
            coily->AddObserver(gameUI);
            coily->AddObserver(manager);
        }
    }

    InputBindingHelper::BindPlayer1KeyboardInputs(qbert.get());
    InputBindingHelper::BindPlayer2CoilyGamepadInputs(coily.get());
}
