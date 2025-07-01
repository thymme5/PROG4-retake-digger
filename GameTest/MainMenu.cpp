#include "MainMenu.h"
#include "QbertSceneBuilder.h"
#include "InputManager.h"

void MainMenu::Enter() 
{
	QbertSceneBuilder::BuildMainMenu(dae::SceneManager::GetInstance().CreateScene(m_SceneName),
		dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36));

	// load all sounds
	auto soundService = dae::ServiceLocator::GetSoundService();
	if (soundService)
	{
		soundService->SetVolume(10);
		QbertSoundLibrary::LoadAllSounds();
	}
}
void MainMenu::Exit() 
{
	dae::InputManager::GetInstance().ClearAllBindings();
	dae::SceneManager::GetInstance().RemoveScene(m_SceneName);
}
void MainMenu::Update() 
{

}