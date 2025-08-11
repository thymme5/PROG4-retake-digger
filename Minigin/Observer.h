#pragma once

namespace dae
{
	class GameObject;

	enum class Event
	{
		PlayerDied,
		PlayerDugTile,
		TileDug,
		TileContainsEmerald,
		TileContainsGoldBag,
		GoldBagDropped,
		GoldCollected,
		EmeraldCollected,
		PlayerCollectedAllEmeralds,
		LevelCompleted,
		EnemyKilled,
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(Event event, GameObject* pGameObject) = 0;
	};
}
