#include "PostGameEntryMode.h"
#include "DiggerSceneBuilder.h"
#include "InputManager.h"
#include "SceneManager.h"

void PostGameEntryMode::Enter()
{
    DiggerSceneBuilder::BuildNameEntryScene(
        dae::SceneManager::GetInstance().CreateScene(m_SceneName)
    );
}

void PostGameEntryMode::Exit()
{
    dae::InputManager::GetInstance().ClearAllBindings();
    dae::SceneManager::GetInstance().MarkSceneForDeletion(m_SceneName);
}

void PostGameEntryMode::Update()
{
   
}
