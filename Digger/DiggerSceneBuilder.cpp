#include "DiggerSceneBuilder.h"
#include "Scene.h"
#include "MainMenuUIComponent.h"
#include "InputManager.h"
#include "MoveMenuArrow.h"
#include "EnterGameMode.h"
#include "HighScoreManager.h"
#include "BackToMenuCommand.h"
#include "FinishRoundCommand.h"
#include "InputManager.h"
#include <sstream>


void DiggerSceneBuilder::BuildMainMenu(dae::Scene& scene, const std::shared_ptr<dae::Font>& font)
{
    auto go = std::make_shared<dae::GameObject>();
    auto UIcomponent = go->AddComponent<MainMenuUIComponent>(*go);
    scene.Add(go);

    const float baseY = 300.0f;
    const float spacing = 50.0f;
    const auto windowWidth = dae::Renderer::GetInstance().GetWindowSize().x;

    std::vector<std::string> options = { "Solo mode", "Co-op Mode", "Versus mode", "High score" };

    float arrowX{};
    float arrowY{};

    for (size_t i = 0; i < options.size(); ++i)
    {
        auto optionGO = std::make_shared<dae::GameObject>();
        auto* text = optionGO->AddComponent<dae::TextComponent>(*optionGO, options[i], font);

        text->Update();

        auto width = text->GetTextureSize().x;
        auto height = text->GetTextureSize().y;

        float x = (windowWidth - width) / 2.0f;
        float y = baseY + i * spacing;


        optionGO->SetLocalPosition(x, y);
        scene.Add(optionGO);

        if (i == 0)
        {
            arrowX = x - 30.f;
            arrowY = y;
        }
    }

    auto arrowGO = std::make_shared<dae::GameObject>();
    arrowGO->AddComponent<dae::TextureComponent>(*arrowGO, "Selection Arrow.png", 2.f);
    arrowGO->SetLocalPosition(arrowX, arrowY);

    const auto& arrowWorldPos = arrowGO->GetTransform().GetWorldPosition();

    UIcomponent->SetArrow(arrowGO);
    scene.Add(arrowGO);

    // Input bindings
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


void DiggerSceneBuilder::BuildHighScoreScene(dae::Scene& scene)
{
    const auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    const auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
    const auto windowWidth = dae::Renderer::GetInstance().GetWindowSize().x;

    // === Title ===
    auto titleGO = std::make_shared<dae::GameObject>();
    auto* titleText = titleGO->AddComponent<dae::TextComponent>(*titleGO, "HIGH SCORES", font);
    titleText->Update();
    float x = (windowWidth - static_cast<float>(titleText->GetTextureSize().x)) / 2.f;
    titleGO->SetLocalPosition(x, 100.f);
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

        scoreGO->SetLocalPosition(x, y);
        scene.Add(scoreGO);
    }

    // === Hint text ===

    auto hintGO = std::make_shared<dae::GameObject>();
    auto* hintText = hintGO->AddComponent<dae::TextComponent>(*hintGO, "ESC OR B TO GO BACK", smallFont);
    hintText->Update();

    x = (windowWidth - static_cast<float>(hintText->GetTextureSize().x)) / 2.f;
    hintGO->SetLocalPosition(x, 400.f);
    scene.Add(hintGO);


    // === Input ===
    auto& input = dae::InputManager::GetInstance();
    auto confirmCmd = std::make_shared<BackToMenuCommand>();
    input.BindCommand(SDLK_ESCAPE, KeyState::Down, confirmCmd);
    input.BindCommand(0, GamepadButton::B, KeyState::Down, confirmCmd);
}

void DiggerSceneBuilder::CreateBaseDiggerScene(dae::Scene& scene, const std::string& levelPath)
{
    //input handling
    auto& inputManager = dae::InputManager::GetInstance();
    
    // === Skip round === 
    auto skipRound = std::make_shared<FinishRoundCommand>();
    inputManager.BindCommand(SDLK_F1, KeyState::Down, skipRound);

	LevelBuilder::LoadLevelFromFile(levelPath, scene);
}

void DiggerSceneBuilder::CreateSinglePlayerScene(dae::Scene& scene, const std::string& levelPath)
{
	CreateBaseDiggerScene(scene, levelPath);
}

void DiggerSceneBuilder::CreateCoopScene(dae::Scene& scene, const std::string& levelPath)
{
    CreateBaseDiggerScene(scene, levelPath);
}


void DiggerSceneBuilder::CreateDebugScene(dae::Scene& scene)
{
    constexpr int TILE_SIZE = 48;
    constexpr int SCREEN_WIDTH = 960;
    constexpr int SCREEN_HEIGHT = 560;

    const int cols = SCREEN_WIDTH / TILE_SIZE;
    const int rows = SCREEN_HEIGHT / TILE_SIZE;

    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            std::string texture = (row + col) % 2 == 0 ? "tile_white.png" : "tile_black.png";
            auto tile = std::make_shared<dae::GameObject>();
            tile->SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);
            tile->AddComponent<dae::TextureComponent>(*tile, texture, 1.5f, 0);
            scene.Add(tile);
        }
    }
}
