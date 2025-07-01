#include "VictoryScreen.h"
#include "QbertSceneBuilder.h"
#include "InputManager.h"

void VictoryScreen::Enter() 
{
	QbertSceneBuilder::BuildVictoryScene(dae::SceneManager::GetInstance().CreateScene(m_SceneName));

}
void VictoryScreen::Exit() 
{
	dae::InputManager::GetInstance().ClearAllBindings();
	dae::SceneManager::GetInstance().MarkSceneForDeletion(m_SceneName);
}
void VictoryScreen::Update() 

{

}