#include "FPScounterComponent.h"
#include "imgui.h"

dae::FPScounterComponent::FPScounterComponent(GameObject& owner):
	Component(owner)
{
	m_LastTime = std::chrono::high_resolution_clock::now();
}

void dae::FPScounterComponent::Update() 
{
	using namespace std::chrono;

	m_FrameCount++;

	auto now = steady_clock::now();
	auto duration = duration_cast<seconds>(now - m_LastTime);

	if (duration.count() >= 0.1)
	{
		m_FPS = m_FrameCount;
		m_FrameCount = 0;
		m_LastTime = now;

		//Get Text component from owner
		TextComponent* textComp = m_pOwner->GetComponent<TextComponent>();

		if (textComp)
		{
			//Modify the text directly without storing it in FPScounterComponent
			//Yes i was causing crashes on exit here
			textComp->SetText(std::to_string(m_FPS) + " FPS");
		}
	}
}
void dae::FPScounterComponent::Render() const 
{
}	