#pragma once
#include "Component.h"
#include "LevelBuilder.h"
#include "QbertSoundLibrary.h"
#include "QbertMoveComponent.h"
#include "CoilyComponent.h"
#include "GameUIComponent.h"
#include "GameModeManager.h"

#include "SinglePlayerMode.h"
#include "CoopMode.h"
#include "VersusMode.h"
#include "Observer.h"
#include "TextureComponent.h"
#include "VictoryScreen.h"
enum class GameState
{
    Playing,
    RoundComplete,
    TransitionToNextRound,
    gameOver,
    gameWon
};

class GameplayManagerComponent final : public dae::Component, public dae::Observer {
public:
    explicit GameplayManagerComponent(dae::GameObject& owner);
    void Init(dae::Scene& scene, const std::string& levelPath = "../data/levels/Level01Solo.json");

    void SetQbert(const std::shared_ptr<dae::GameObject>& qbert) { m_pQbert = qbert; }
    void SetCoily(const std::shared_ptr<dae::GameObject>& coily) { m_pCoily = coily; }

    void SetGameUI(dae::GameUIComponent* gameUI);
    dae::GameUIComponent* GetGameUI() const;
    

    GameState GetCurrentState() const noexcept;

    void OnNotify(dae::Event event, dae::GameObject* pGameObject) override;
    ~GameplayManagerComponent() override = default;

    static GameplayManagerComponent* GetInstance();
    static void SetInstance(GameplayManagerComponent* instance);

    void Update() override;
    void Render() const override {};

    void ForceCompleteRound();
    void SetNextLevel();

    void RespawnQbert();
private:
    void CheckRoundComplete();
    void StopTileFlashing();
    void StartNextRound();

    GameState m_CurrentState{};
    float m_StateTimer{};
    bool m_RoundInProgress{ true };

    dae::Scene* m_pScene = nullptr;
    std::string m_LevelPath{};
    
    int m_CurrentRoundIndex = 0;
    int m_CurrentLevelIndex = 1;
    
    //pointers to entities
    std::weak_ptr<dae::GameObject> m_pQbert; // in multiplayer modes gameplaymanager component will still use qbert 1 as a reference - which shouldn't create any gameplay differences, but also doesn't require for gameplaymanager to be aware of all qbert entities
    std::weak_ptr<dae::GameObject> m_pCoily;

    dae::GameUIComponent* m_GameUIComponent;

    inline static GameplayManagerComponent* s_Instance = nullptr;

    float m_ShowSwearTimer{};

    bool m_ShouldRespawnQbert{false};
   

};
