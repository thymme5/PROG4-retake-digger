#pragma once
#include <string>
#include <utility>
#include <memory>
#include "GameObject.h"
#include "TextureComponent.h"

namespace dae
{
	class Scene;
}

class LevelBuilder final
{
public:
	static void LoadLevelFromFile(const std::string& path, dae::Scene& scene);
private:
    static std::shared_ptr<dae::GameObject> CreateBasicGO(dae::Scene& scene, std::pair<int, int> gridPos, const std::string& texturePath);
    static void SpawnDirt(dae::Scene& scene, std::pair<int, int> gridPos);
    static void SpawnEmerald(dae::Scene& scene, std::pair<int, int> gridPos);
    static void SpawnGoldBag(dae::Scene& scene, std::pair<int, int> gridPos);
    static void SpawnPlayer(dae::Scene& scene, std::pair<int, int> gridPos);
    static void SpawnNobbin(dae::Scene& scene, std::pair<int, int> gridPos);
    static void SpawnHobbin(dae::Scene& scene, std::pair<int, int> gridPos);
};
