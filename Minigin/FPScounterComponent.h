#pragma once
#include "component.h"
#include "TextComponent.h"
#include <chrono>

namespace dae
{
	class GameObject;
	class FPScounterComponent : public Component
	{
	public:
		FPScounterComponent(GameObject& owner);

		void Update() override;
		void Render() const override;

	private:
		int m_FPS = 0;
		int m_FrameCount = 0;
		std::chrono::time_point<std::chrono::steady_clock> m_LastTime;
	};
}
