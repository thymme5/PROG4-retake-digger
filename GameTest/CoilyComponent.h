#pragma once
#include "Component.h"
#include "CoilyState.h"
#include "GameObject.h"
#include "TileComponent.h"
#include "QbertMoveComponent.h"
#include "QbertSoundLibrary.h"
#include "ChasingState.h"
#include "EggState.h"
#include "TextureComponent.h"
#include "GameplayManagerComponent.h"


struct CoilyJumpData
{
    glm::vec2 startPos;
    glm::vec2 endPos;
    bool isJumping = false;
    float elapsed = 0.f;
    float duration = 0.8f; //TODO: tweak this value
    float waitTimer = 0.f;
    float waitDuration = 0.4f;
};

class TileComponent;

class CoilyComponent final : public dae::Component
{
public:
    explicit CoilyComponent(dae::GameObject& owner);
    ~CoilyComponent() override;

    void Update() override;
    void Render() const override;

    void SetState(std::unique_ptr<CoilyState> newState);
    const CoilyState* GetState() const noexcept;

    //tile functions
    void SetCurrentTile(std::shared_ptr<TileComponent> tile);
    void SetTileMap(const std::vector<std::vector<std::shared_ptr<TileComponent>>>& tileMap);
    std::shared_ptr<TileComponent> GetCoilyTile();

    //Get pointer to qbert's ass
    void SetQbert(std::shared_ptr<dae::GameObject> qbert);
    const QbertMoveComponent* GetQbert();
    std::shared_ptr<TileComponent> GetQbertTile();
    
    bool IsJumping() const;
    bool GetPaused() const noexcept;
    bool GetIsPlayer() const noexcept;
    void SetPaused(bool paused);

    void TryMove(Direction direction);
    glm::vec3 GetPosition() const;

    void SetPlayerControlled(bool isControlled);
    bool IsPlayerControlled() const noexcept;

private:
    void SetJumpSprite(Direction dir);

    Direction m_LastMoveDir;

    //pointer to qbert
    std::shared_ptr<dae::GameObject> m_pQbert;

    //state
    std::unique_ptr<CoilyState> m_pCurrentState = nullptr;

    //incorporate him into the tile mechanic
    std::shared_ptr<TileComponent> m_CurrentTile;
    const std::vector<std::vector<std::shared_ptr<TileComponent>>>* m_pTileMap = nullptr;

    //slight offset to center them in tile
    const float m_xOffset = 15.f; //magic number but pre-calculated value (kevin would love this)
    const float m_yOffset = -40.f; //magic number but pre-calculated value

    CoilyJumpData m_Jump;

    bool m_IsPlayerControlled{ false };

    bool m_IsPaused;

};
