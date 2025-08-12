#include "DiggerSceneBuilder.h"
#include "Scene.h"
#include "MainMenuUIComponent.h"
#include "InputManager.h"
#include "MoveMenuArrow.h"
#include "EnterGameMode.h"

void DiggerSceneBuilder::BuildMainMenu(dae::Scene& scene, const std::shared_ptr<dae::Font>& font)
{
    auto go = std::make_shared<dae::GameObject>();
    auto UIcomponent = go->AddComponent<MainMenuUIComponent>(*go);
    scene.Add(go);

    const float baseY = 300.0f;
    const float spacing = 50.0f;
    const auto windowWidth = dae::Renderer::GetInstance().GetWindowSize().x;

    std::cout << "[DEBUG] Window width: " << windowWidth << std::endl;

    std::vector<std::string> options = { "Solo mode", "Co-op Mode", "Versus mode", "High score" };

    float arrowX{};
    float arrowY{};

    for (size_t i = 0; i < options.size(); ++i)
    {
        auto optionGO = std::make_shared<dae::GameObject>();
        auto* text = optionGO->AddComponent<dae::TextComponent>(*optionGO, options[i], font);

        text->SetText(options[i]);
        text->Update();

        auto width = text->GetTextureSize().x;
        auto height = text->GetTextureSize().y;

        std::cout << "[DEBUG] Option " << i << " \"" << options[i] << "\" texture size: "
            << width << "x" << height << std::endl;

        float x = (windowWidth - width) / 2.0f;
        float y = baseY + i * spacing;

        std::cout << "[DEBUG] Option " << i << " position: (" << x << ", " << y << ")" << std::endl;

        optionGO->SetLocalPosition(x, y);
        scene.Add(optionGO);

        if (i == 0)
        {
            arrowX = x - 30.f;
            arrowY = y;
            std::cout << "[DEBUG] Arrow initial position: (" << arrowX << ", " << arrowY << ")" << std::endl;
        }
    }

    auto arrowGO = std::make_shared<dae::GameObject>();
    arrowGO->AddComponent<dae::TextureComponent>(*arrowGO, "Selection Arrow.png", 2.f);
    arrowGO->SetLocalPosition(arrowX, arrowY);

    const auto& arrowWorldPos = arrowGO->GetTransform().GetWorldPosition();
    std::cout << "[DEBUG] Arrow world position: (" << arrowWorldPos.x << ", " << arrowWorldPos.y << ")" << std::endl;

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
void DiggerSceneBuilder::CreateBaseDiggerScene(dae::Scene& scene, const std::string& levelPath)
{
	LevelBuilder::LoadLevelFromFile(levelPath, scene);
}

void DiggerSceneBuilder::CreateSinglePlayerScene(dae::Scene& scene, const std::string& levelPath)
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
