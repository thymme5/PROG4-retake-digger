#include "VersusMode.h"
#include "QbertSceneBuilder.h"
#include "ResourceManager.h"

#include <filesystem>
#include <sstream>

VersusMode::VersusMode(int levelIndex) :
    m_LevelIndex(levelIndex)
{
}

void VersusMode::Enter()
{
    const auto& basePath = dae::ResourceManager::GetInstance().GetDataPath();

    std::stringstream ss;
    ss << (basePath / "levels" / ("Level0" + std::to_string(m_LevelIndex) + "Versus.json")).string();
    std::string levelPath = ss.str();

    QbertSceneBuilder::BuildVersusScene(
        dae::SceneManager::GetInstance().CreateScene(m_SceneName),
        levelPath
    );
}

void VersusMode::Exit()
{
    dae::SceneManager::GetInstance().MarkSceneForDeletion(m_SceneName);
}

void VersusMode::Update()
{
}
