#pragma once
#include "TileComponent.h"

#include <memory>
#include <glm.hpp>
#include <vector>
#include <nlohmann/json.hpp>
#include <string>
#include "ColorMap.h"

// Coordinate system used for moving entities
//
//                               (0,0)
//                          (1,0)     (1,1)
//                     (2,0)     (2,1)     (2,2)
//                (3,0)     (3,1)     (3,2)     (3,3)
//           (4,0)     (4,1)     (4,2)     (4,3)     (4,4)
//      (5,0)     (5,1)     (5,2)     (5,3)     (5,4)     (5,5)
// (6,0)     (6,1)     (6,2)     (6,3)     (6,4)     (6,5)     (6,6)


namespace dae
{
	class Scene;
	class GameObject;
}

class LevelBuilder
{
public:
	static void LoadFromJson(dae::Scene& scene, const std::string& pathToJson, int roundNumber);

	static std::shared_ptr<dae::GameObject> CreateTile(int id, const glm::vec2& pos, const std::string& color);
	static const std::vector<std::vector<std::shared_ptr<dae::GameObject>>>& GetTileMap();
	static const std::vector<std::vector<std::shared_ptr<TileComponent>>>& GetTileComponentMap();
	static std::vector<std::pair<int, int>> GetStartTileIndices();
private:
	static std::vector<std::vector<std::shared_ptr<dae::GameObject>>> m_TilesByRow;
	inline static std::vector<std::pair<int, int>> m_StartTiles{};
};
