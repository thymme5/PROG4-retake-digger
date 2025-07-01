#include "SinglePlayerMode.h"
#include "QbertSceneBuilder.h"
#include "InputManager.h"

SinglePlayerMode::SinglePlayerMode(int levelIndex)
    :
    m_LevelIndex(levelIndex)
{
    
}

void SinglePlayerMode::Enter()
{
    std::stringstream ss;
    ss << dae::ResourceManager::GetInstance().GetDataPath().string() << "/levels/Level0" << m_LevelIndex << "Solo.json";
    std::string levelPath = ss.str();

    QbertSceneBuilder::BuildSinglePlayerScene(dae::SceneManager::GetInstance().CreateScene(m_SceneName), levelPath);
}

void SinglePlayerMode::Exit()
{
    dae::InputManager::GetInstance().ClearAllBindings();
    dae::SceneManager::GetInstance().MarkSceneForDeletion(m_SceneName);
}

void SinglePlayerMode::Update()
{

}
