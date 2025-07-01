#include "GameplayManagerComponent.h"
#include "TileComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include <fstream>

GameplayManagerComponent::GameplayManagerComponent(dae::GameObject& owner)
    : Component(owner)
{
}
void GameplayManagerComponent::Init(dae::Scene& scene, const std::string& levelPath)
{
    m_pScene = &scene;
    m_LevelPath = levelPath;
    m_CurrentRoundIndex = 1;

    LevelBuilder::LoadFromJson(scene, m_LevelPath, m_CurrentRoundIndex);
}
void GameplayManagerComponent::Update() 
{
    constexpr float deltaTime = 1.0f / 60.0f;

    switch (m_CurrentState) {
    case GameState::Playing:
        if (m_RoundInProgress)
            CheckRoundComplete();

        if (m_ShowSwearTimer > 0.0f)
        {
            m_ShowSwearTimer -= deltaTime;

            if (m_ShowSwearTimer <= 0.0f && m_ShouldRespawnQbert)
            {
                if (auto qbert = m_pQbert.lock())
                {
                    if (auto* swear = qbert->FindChildWithComponent<dae::TextureComponent>())
                        swear->GetComponent<dae::TextureComponent>()->SetVisible(false);
                }

                RespawnQbert();
                m_ShouldRespawnQbert = false;

                if (auto coily = m_pCoily.lock())
                {
                    if (auto* coilyComp = coily->GetComponent<CoilyComponent>())
                        coilyComp->SetPaused(false);
                }
            }
        }

        break;

    case GameState::RoundComplete:
        if (!QbertSoundLibrary::IsPlaying(SoundID::RoundComplete))
        {
            QbertSoundLibrary::Play(SoundID::RoundComplete);
        }

        m_StateTimer += deltaTime;

        for (auto& row : LevelBuilder::GetTileComponentMap()) 
        {
            for (auto& tile : row)
                tile->Update();
        }

        if (m_StateTimer >= 2.5f) 
        {
            StopTileFlashing();
            StartNextRound();

            m_CurrentState = GameState::Playing;
            m_StateTimer = 0.0f;
            m_RoundInProgress = true;
        }
        break;

    case GameState::TransitionToNextRound:
        break;
    }
}
void GameplayManagerComponent::SetGameUI(dae::GameUIComponent* gameUI)
{
    m_GameUIComponent = gameUI;
}
dae::GameUIComponent* GameplayManagerComponent::GetGameUI() const
{
    return m_GameUIComponent;
}
void GameplayManagerComponent::CheckRoundComplete()
{
    bool allDone = true;

    for (const auto& row : LevelBuilder::GetTileComponentMap())
    {
        for (const auto& tile : row) 
        {
            if (!tile->IsCompleted()) 
            {
                allDone = false;
                break;
            }
        }
        if (!allDone) break;
    }

    if (allDone)
    {
        m_CurrentState = GameState::RoundComplete;
        m_StateTimer = 0.0f;
        m_RoundInProgress = false;

        if (auto coily = m_pCoily.lock())
        {
            auto* coilyMoveComponent = coily->GetComponent<CoilyComponent>();
            coilyMoveComponent->SetPaused(true);
        }

        for (const auto& row : LevelBuilder::GetTileComponentMap())
        {
            for (const auto& tile : row)
                tile->StartFlashing();
        }
    }
}

void GameplayManagerComponent::StopTileFlashing()
{
    for (const auto& row : LevelBuilder::GetTileComponentMap())
    {
        for (const auto& tile : row)
            tile->StopFlashing();
    }
}

void GameplayManagerComponent::StartNextRound()
{
    if (m_CurrentRoundIndex < 3)
    {
        ++m_CurrentRoundIndex;

        //count levels
        std::ifstream file(m_LevelPath);
        if (!file.is_open())
        {
            std::cerr << "[GameplayManager] Failed to open level JSON: " << m_LevelPath << '\n';
            return;
        }

        nlohmann::json levelData;
        file >> levelData;

        int maxRounds = static_cast<int>(levelData["rounds"].size());

        if (m_CurrentRoundIndex > maxRounds)
        {
            std::cout << "[GameplayManager] All rounds completed. Level finished.\n";
            m_RoundInProgress = false;
            m_CurrentState = GameState::Playing;
            m_pOwner->NotifyObservers(dae::Event::LevelComplete);

            return;
        }

        std::cout << "[GameplayManager] Starting round " << m_CurrentRoundIndex << '\n';
        m_pOwner->NotifyObservers(dae::Event::roundComplete);

        // remove old tile gameobjects
        m_pScene->RemoveObjectsWithComponent<TileComponent>();

        // load new tiles for the next round (same level)
        LevelBuilder::LoadFromJson(*m_pScene, m_LevelPath, m_CurrentRoundIndex);

        const auto& newTileMap = LevelBuilder::GetTileMap();
        auto firstTileGO = newTileMap[0][0]; 
        TileComponent* rawComp = firstTileGO->GetComponent<TileComponent>();
        std::shared_ptr<TileComponent> firstTileComp(firstTileGO, rawComp); 

        // update Qbert
        if (auto qbert = m_pQbert.lock())
        {
            auto* qbertMoveComponent = qbert->GetComponent<QbertMoveComponent>();

            qbertMoveComponent->SetTileMap(LevelBuilder::GetTileComponentMap());
            qbertMoveComponent->SetCurrentTile(firstTileComp);
        }

        //reset enemies (only coily for now) 
        //TODO: maybe refactor when we have more enemies
        if (auto coily = m_pCoily.lock())
        {
            auto* coilyMoveComponent = coily->GetComponent<CoilyComponent>();
            coilyMoveComponent->SetState(std::make_unique<EggState>());
            coilyMoveComponent->SetTileMap(LevelBuilder::GetTileComponentMap());
            coilyMoveComponent->SetCurrentTile(firstTileComp);
            coilyMoveComponent->SetPaused(false);

        }

        // set state flags
        m_CurrentState = GameState::Playing;
        m_RoundInProgress = true;
    }
    else
    {
        //game won logic 
        HighscoreEntry newEntry{ "YOU", m_GameUIComponent->GetScore() };
        HighscoreManager::GetInstance().AddHighscore(newEntry);
        HighscoreManager::GetInstance().SaveHighscores();

        GameModeManager::GetInstance().SetMode(std::make_unique<VictoryScreen>());

        return;
    }
}

void GameplayManagerComponent::SetNextLevel()
{
    ++m_CurrentLevelIndex;
    m_CurrentRoundIndex = 1;

        auto* currentMode = GameModeManager::GetInstance().GetCurrentGameMode();
    if (dynamic_cast<SinglePlayerMode*>(currentMode))
    {
        GameModeManager::GetInstance().SetMode(std::make_unique<SinglePlayerMode>(m_CurrentLevelIndex));
    }
    else if (dynamic_cast<CoopMode*>(currentMode))
    {
        GameModeManager::GetInstance().SetMode(std::make_unique<CoopMode>(m_CurrentLevelIndex));
    }
    else if (dynamic_cast<VersusMode*>(currentMode))
    {
        GameModeManager::GetInstance().SetMode(std::make_unique<VersusMode>(m_CurrentLevelIndex));
    }
}

void GameplayManagerComponent::ForceCompleteRound()
{
    if (m_CurrentState != GameState::Playing)
        return;

    if (auto coily = m_pCoily.lock())
    {
        if (auto* coilyComp = coily->GetComponent<CoilyComponent>())
            coilyComp->SetPaused(false);
    }

    m_CurrentState = GameState::RoundComplete;
    m_StateTimer = 0.0f;
    m_RoundInProgress = false;

    for (const auto& row : LevelBuilder::GetTileComponentMap())
    {
        for (const auto& tile : row)
        {
            tile->SetState(TileState::Target);
            tile->StartFlashing();
        }
    }
}
void GameplayManagerComponent::OnNotify(dae::Event event, dae::GameObject* /*pGameObject*/)
{
    if (m_ShouldRespawnQbert)
        return;

    if (event == dae::Event::CoilyHitPlayer)
    {
        // update UI lives
        if (m_GameUIComponent)
        {
            m_GameUIComponent->UpdateLives();
            if (m_GameUIComponent->GetLives() == 0)

            {
                HighscoreEntry newEntry{ "YOU", m_GameUIComponent->GetScore()};
                HighscoreManager::GetInstance().AddHighscore(newEntry);

                HighscoreManager::GetInstance().SaveHighscores();

                GameModeManager::GetInstance().SetMode(std::make_unique<GameOverMenu>());
                return;

            }
        }
            
        // show Qbert's swearing icon
        if (auto qbert = m_pQbert.lock())
        {
            auto* swearGO = qbert->FindChildWithComponent<dae::TextureComponent>();
            if (swearGO)
            {
                if (auto* tex = swearGO->GetComponent<dae::TextureComponent>())
                    tex->SetVisible(true);
            }
        }

        // pause Coily's behavior
        if (auto coily = m_pCoily.lock())
        {
            if (auto* coilyComp = coily->GetComponent<CoilyComponent>())
                coilyComp->SetPaused(true);
        }


        // Set timer and play sound
        m_ShowSwearTimer = 2.0f;
        m_ShouldRespawnQbert = true;
        QbertSoundLibrary::Play(SoundID::Swearing);
    }
}

void GameplayManagerComponent::RespawnQbert()
{
    const auto& tileMap = LevelBuilder::GetTileComponentMap();
    if (tileMap.empty() || tileMap[0].empty())
        return;

    auto startTile = tileMap[0][0];

    if (auto qbert = m_pQbert.lock())
    {
        if (auto* moveComp = qbert->GetComponent<QbertMoveComponent>())
        {
            moveComp->SetTileMap(tileMap);
            moveComp->SetCurrentTile(startTile); 
        }
    }
}

GameState GameplayManagerComponent::GetCurrentState() const noexcept
{
    return m_CurrentState;
}

GameplayManagerComponent* GameplayManagerComponent::GetInstance()
{
    return s_Instance;
}
 void GameplayManagerComponent::SetInstance(GameplayManagerComponent* instance)
{
     s_Instance = instance;
}