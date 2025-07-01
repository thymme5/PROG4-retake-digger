#include "LevelBuilder.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "TextureComponent.h"
#include "TextComponent.h"
#include "TileComponent.h"
#include <fstream>

std::vector<std::vector<std::shared_ptr<dae::GameObject>>> LevelBuilder::m_TilesByRow{};

void LevelBuilder::LoadFromJson(dae::Scene& scene, const std::string& pathToJson, int roundNumber)
{
    using json = nlohmann::json;

    std::ifstream file(pathToJson);
    if (!file.is_open())
        throw std::runtime_error("Failed to open level json: " + pathToJson);

    json data;
    file >> data;

    constexpr float tileSize = 64.0f;
    constexpr float startX = 288.f;
    constexpr float startY = 64.f;

    // === Load basic level structure ===
    int tileCount = data["tileCount"].get<int>();
    m_TilesByRow.clear();
    m_TilesByRow.resize(tileCount);

    // === decide start tiles based on mode ===
    m_StartTiles.clear();
    std::string mode = data.value("mode", "Solo");

    if (mode == "Solo" && data.contains("startTile"))
    {
        const auto& start = data["startTile"];
        m_StartTiles.emplace_back(start[0], start[1]);
    }
    else if ((mode == "Coop" || mode == "Versus") && data.contains("startTiles"))
    {
        for (const auto& start : data["startTiles"])
            m_StartTiles.emplace_back(start[0], start[1]);
    }
    else
    {
        throw std::runtime_error("Missing or invalid start tile(s) for mode: " + mode);
    }

    // === Find the correct round entry ===
    const auto& rounds = data["rounds"];
    const json* round = nullptr;

    for (const auto& r : rounds)
    {
        if (r.contains("round") && r["round"].get<int>() == roundNumber)
        {
            round = &r;
            break;
        }
    }

    if (!round)
        throw std::runtime_error("Round number " + std::to_string(roundNumber) + " not found.");

    // === Extract round color data and rule ===
    std::string startColor = (*round)["startColor"];
    std::string targetColor = (*round)["targetColor"];
    std::string intermediateColor{};

    TileRule rule = TileRule::OneStepToTarget;
    const std::string jumpMode = round->value("jumpMode", "single");

    if (jumpMode == "double")
    {
        if (!round->contains("intermediateColor"))
            throw std::runtime_error("jumpMode is 'double' but 'intermediateColor' is missing.");
        intermediateColor = (*round)["intermediateColor"];
        rule = TileRule::TwoStepsToTarget;
    }
    else if (jumpMode == "toggle")
    {
        rule = TileRule::ToggleColor;
    }

    // === Create all tiles ===
    int id = 0;
    for (int row = 0; row < tileCount; ++row)
    {
        for (int col = 0; col <= row; ++col)
        {
            glm::vec2 pos = {
                startX + (static_cast<float>(col) - static_cast<float>(row) / 2.0f) * tileSize,
                startY + static_cast<float>(row) * tileSize * 0.75f
            };

            auto tile = CreateTile(id++, pos, startColor);
            auto* tileComp = tile->GetComponent<TileComponent>();
            tileComp->SetGridPosition(row, col);
            tileComp->SetColorStates(startColor, intermediateColor, targetColor);
            tileComp->SetTileRule(rule);

            m_TilesByRow[row].push_back(tile);
            scene.Add(tile);
        }
    }
}

std::shared_ptr<dae::GameObject> LevelBuilder::CreateTile(int id, const glm::vec2& pos, const std::string& color)
{
	//get all them bitches
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto tile = std::make_shared<dae::GameObject>();

	// Load the full tile spritesheet
	auto textureComp = tile->AddComponent<dae::TextureComponent>(*tile, "Qbert Cubes.png", 2.0f);

	// use the color argument to select the correct sprite clip from the map
	const SDL_Rect& srcRect = g_ColorSpriteMap.at(color);
	textureComp->SetSourceRect(srcRect);

	// debug text (empty)
	tile->AddComponent<dae::TextComponent>(*tile, " ", font);

	// tile logic
	auto* tileComp = tile->AddComponent<TileComponent>(*tile);
	tileComp->SetID(id);
	tileComp->SetState(TileState::Default);
	tileComp->SetTargetState(TileState::Target);

	tile->SetPosition(pos.x, pos.y);

	return tile;
}
const std::vector<std::vector<std::shared_ptr<dae::GameObject>>>& LevelBuilder::GetTileMap()
{
	return m_TilesByRow;
}
const std::vector<std::vector<std::shared_ptr<TileComponent>>>& LevelBuilder::GetTileComponentMap()
{
	static std::vector<std::vector<std::shared_ptr<TileComponent>>> tileComponentMap{};
	tileComponentMap.clear(); // bug fix of the row looping around

	for (const auto& row : m_TilesByRow)
	{
		std::vector<std::shared_ptr<TileComponent>> compRow;

		for (const auto& tileGO : row)
		{
			auto raw = tileGO->GetComponent<TileComponent>();

			if (raw)
			{
				auto shared = std::shared_ptr<TileComponent>(raw, [](TileComponent*) {});
				compRow.push_back(shared);
			}
		}
		tileComponentMap.push_back(std::move(compRow));
	}
	return tileComponentMap;
}


std::vector<std::pair<int, int>> LevelBuilder::GetStartTileIndices()
{
	return m_StartTiles;
}