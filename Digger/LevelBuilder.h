#pragma once
#include <string>
#include <utility>
#include <memory>
#include "GameObject.h"
#include "TextureComponent.h"

#include "TextureComponent.h"
#include "Scene.h"
#include "TileComponent.h"
#include "TileManager.h"
#include "SubjectComponent.h"
#include "PlayerComponent.h"
#include "AliveState.h"
#include "EmeraldComponent.h"
#include "GoldBagComponent.h"
#include "UIComponent.h"
#include "NobbinState.h"
#include "EnemyComponent.h"

namespace dae
{
	class Scene;
}

class LevelBuilder final
{
public:
	static void LoadLevelFromFile(const std::string& path, dae::Scene& scene);
private:
	static void SpawnPlayers(const std::vector<std::vector<int>>& spawns, dae::Scene& scene, UIComponent* ui, const std::string& mode);
};
