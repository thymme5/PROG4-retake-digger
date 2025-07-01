#include "CoopMode.h"
#include "QbertSceneBuilder.h"
#include "ResourceManager.h"

#include <filesystem>
#include <sstream>

CoopMode::CoopMode(int levelIndex) :
    m_LevelIndex(levelIndex)
{
}

void CoopMode::Enter()
{
    const auto& basePath = dae::ResourceManager::GetInstance().GetDataPath();

    std::stringstream ss;
    ss << (basePath / "levels" / ("Level0" + std::to_string(m_LevelIndex) + "Coop.json")).string();
    std::string levelPath = ss.str();

    QbertSceneBuilder::BuildCoopScene(
        dae::SceneManager::GetInstance().CreateScene(m_SceneName),
        levelPath
    );
}

void CoopMode::Exit()
{
    dae::SceneManager::GetInstance().MarkSceneForDeletion(m_SceneName);
}

void CoopMode::Update()
{

}
