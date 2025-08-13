#include "DeadState.h"
#include "PlayerComponent.h"
#include "TextureComponent.h"
#include "Timer.h"
#include "AliveState.h"

constexpr int TILE_SIZE = 48;

void DeadState::Enter(PlayerComponent& player)
{
    m_DeathTimer = 0.f;
    m_PulseTimer = 0.f;
    m_IsVisible = true;

    if (auto* tex = player.GetOwner()->GetComponent<dae::TextureComponent>())
        tex->SetVisible(true);
}

void DeadState::Update(PlayerComponent& player)
{
    m_DeathTimer += Timer::GetDeltaTime();
    m_PulseTimer += Timer::GetDeltaTime();

    if (m_PulseTimer >= m_PulseInterval)
    {
        m_IsVisible = !m_IsVisible;
        m_PulseTimer = 0.f;

        if (auto* tex = player.GetOwner()->GetComponent<dae::TextureComponent>())
            tex->SetVisible(m_IsVisible);
    }

    if (m_DeathTimer >= m_RespawnDelay)
    {
        auto [row, col] = player.GetSpawnPosition();
        player.SetTilePosition(row, col);

        player.GetOwner()->SetLocalPosition(player.GetCol() * TILE_SIZE, player.GetRow() * TILE_SIZE);
        player.SetState(std::make_unique<AliveState>());
    }
}

void DeadState::Exit(PlayerComponent& player)
{
    m_DeathTimer = 0.f;
    m_PulseTimer = 0.f;
    m_IsVisible = true;

    if (auto* tex = player.GetOwner()->GetComponent<dae::TextureComponent>())
        tex->SetVisible(true);
}