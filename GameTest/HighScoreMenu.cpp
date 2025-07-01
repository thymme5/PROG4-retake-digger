#include "HighScoreMenu.h"
#include "QbertSceneBuilder.h"
#include "InputManager.h"

void HighScoreMenu::Enter()
{
    QbertSceneBuilder::BuildHighScoreScene(dae::SceneManager::GetInstance().CreateScene(m_SceneName));
}
void HighScoreMenu::Exit() 
{
    dae::InputManager::GetInstance().ClearAllBindings();
    dae::SceneManager::GetInstance().MarkSceneForDeletion(m_SceneName);
}
void HighScoreMenu::Update()
{

}
