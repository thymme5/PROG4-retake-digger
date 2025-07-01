#include "GameOverMenu.h"
#include "QbertSceneBuilder.h"
#include "InputManager.h"

void GameOverMenu::Enter()
{
	QbertSceneBuilder::BuildGameOverScene(dae::SceneManager::GetInstance().CreateScene(m_SceneName));
}
void GameOverMenu::Exit()
{
	dae::InputManager::GetInstance().ClearAllBindings();
	dae::SceneManager::GetInstance().MarkSceneForDeletion(m_SceneName);
}
void GameOverMenu::Update()
{

}