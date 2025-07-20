#include "MainMenu.h"
#include "InputManager.h"
#include "SceneManager.h"

void MainMenu::Enter()
{
	// load all sounds
	//auto soundService = dae::ServiceLocator::GetSoundService();
	//if (soundService)
	//{
	//	soundService->SetVolume(10);
	//}
}
void MainMenu::Exit()
{
	dae::InputManager::GetInstance().ClearAllBindings();
	dae::SceneManager::GetInstance().RemoveScene(m_SceneName);
}
void MainMenu::Update()
{

}