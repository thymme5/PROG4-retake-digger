#include "HighScoreMenu.h"
#include "DiggerSceneBuilder.h"
#include "InputManager.h"
#include "SceneManager.h"
void HighScoreMenu::Enter()
{
    DiggerSceneBuilder::BuildHighScoreScene(dae::SceneManager::GetInstance().CreateScene(m_SceneName));
}
void HighScoreMenu::Exit()
{
    dae::InputManager::GetInstance().ClearAllBindings();
    dae::SceneManager::GetInstance().MarkSceneForDeletion(m_SceneName);
}
void HighScoreMenu::Update()
{

}
