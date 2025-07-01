#pragma once

namespace dae
{
	class GameObject;

	enum class Event
	{
		PlayerDied,
		PlayerLandedOnTile,
		TileStateChanged,
		PlayerWonGame,
		roundComplete,
		LevelComplete,
		CoilyHitPlayer
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(Event event, GameObject* pGameObject) = 0;
	};
}
