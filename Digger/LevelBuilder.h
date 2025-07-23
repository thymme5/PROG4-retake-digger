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
};
