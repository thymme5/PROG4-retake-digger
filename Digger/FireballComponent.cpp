#include "FireballComponent.h"
#include "TileManager.h"
#include "GameObject.h"
#include "TileComponent.h"
#include "TextureComponent.h"

constexpr int TILE_SIZE = 48;

FireballComponent::FireballComponent(dae::GameObject& owner, int row, int col, int dRow, int dCol)
    : Component(owner), m_Row{ row }, m_Col{ col }, m_DirRow{ dRow }, m_DirCol{ dCol }
{
    owner.SetLocalPosition(col * TILE_SIZE, row * TILE_SIZE);
}

void FireballComponent::Update()
{
    m_Timer += 1.f / 60.f;
    if (m_Timer < m_MoveInterval) return;
    m_Timer = 0.f;

    int nextRow = m_Row + m_DirRow;
    int nextCol = m_Col + m_DirCol;

    auto tile = TileManager::GetInstance().GetTile(nextRow, nextCol);
    if (!tile || !tile->IsDug())
    {
        GetOwner()->Destroy();
        return;
    }

    auto interactables = TileManager::GetInstance().GetInteractablesAt(nextRow, nextCol);
    for (auto* obj : interactables)
    {
        //if (obj->HasComponent<EnemyComponent>()) // placeholder
        //{
        //    obj->Destroy();
        //    GetOwner()->Destroy();
        //    return;
        //}
    }

    m_Row = nextRow;
    m_Col = nextCol;
    GetOwner()->SetLocalPosition(m_Col * TILE_SIZE, m_Row * TILE_SIZE);
}
void FireballComponent::Render() const
{

}