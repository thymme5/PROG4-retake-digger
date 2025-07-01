#pragma once
#include "Component.h"
#include <utility>
#include <string>

// Coordinate system used for moving entities
//
//                               (0,0)
//                          (1,0)     (1,1)
//                     (2,0)     (2,1)     (2,2)
//                (3,0)     (3,1)     (3,2)     (3,3)
//           (4,0)     (4,1)     (4,2)     (4,3)     (4,4)
//      (5,0)     (5,1)     (5,2)     (5,3)     (5,4)     (5,5)
// (6,0)     (6,1)     (6,2)     (6,3)     (6,4)     (6,5)     (6,6)

enum class TileState
{
	Default,
	Intermediate,
	Target
};
enum class TileRule
{
	OneStepToTarget,      // Level 1 and 3
	TwoStepsToTarget,     // Level 2
	ToggleColor           // Level 3 only
};
class TileComponent final : public dae::Component
{
public:
	explicit TileComponent(dae::GameObject& owner);
	~TileComponent() override = default;

	void Update() override;
	void Render() const override;

	void SetID(int id);
	int GetID() const;

	void SetState(TileState state);
	TileState GetState() const;

	void SetTargetState(TileState targetState);
	bool IsCompleted() const;

	void OnStepped(dae::GameObject* actor);

	void SetColorStates(const std::string& startColor, const std::string& intermediateColor, const std::string& targetColor);
	std::string GetCurrentColor() const;

	// grid position (coord system)
	void SetGridPosition(int row, int col);
	std::pair<int, int> GetGridPosition() const;

	//for the cool game ending animation
	void StartFlashing();
	void StopFlashing();
	void SetTileRule(TileRule rule);
private:
	int m_ID{}; // basically index
	TileState m_CurrentState{ TileState::Default };
	TileState m_TargetState{ TileState::Target };
	bool m_IsCompleted{ false };

	int m_Row{};
	int m_Col{};

	TileRule m_TileRule{ TileRule::OneStepToTarget };
	int m_JumpCount{ 0 };

	std::string m_StartColor{};
	std::string m_IntermediateColor{};
	std::string m_TargetColor{};

	//game ending animation
	bool m_IsFlashing{ false };
	float m_FlashTimer{ 0.0f };
	float m_FlashInterval{ 0.2f };
	bool m_FlashVisibleState{ true };
};
