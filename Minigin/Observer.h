#pragma once

namespace dae
{
	class GameObject;

	enum class Event
	{
		PlayerDied,
		EnemyKilled,
		
		PlayerDugTile,
		TileDug,
		
		TileContainsEmerald,
		TileContainsGoldBag,
		
		GoldBagDropped,
		GoldCollected,
		
		EmeraldCollected,
		PlayerCollected8Emeralds,
		PlayerCollectedAllEmeralds,
		
		LevelCompleted,
		GameCompleted,
		GameOver
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(Event event, GameObject* pGameObject) = 0;
	};
}
