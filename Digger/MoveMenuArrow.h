#pragma once
#include "Command.h"
#include "MainMenuUIComponent.h"
#include <memory>

class MoveMenuArrow final : public dae::Command
{
public:
	MoveMenuArrow(MainMenuUIComponent* pMenuUI, float direction)
		: m_pMenuUI(pMenuUI), m_Direction(direction) {
	}

	void Execute() override
	{
		const float spacing = 50.f;
		m_pMenuUI->MoveArrow(m_Direction * spacing);
	}

private:
	MainMenuUIComponent* m_pMenuUI;
	float m_Direction{}; // -1 for down 1 for up
};
