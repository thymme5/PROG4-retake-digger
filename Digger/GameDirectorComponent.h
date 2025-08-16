#include "Component.h"
#include "GameObject.h"
#include "GameModeManager.h"

class GameDirectorComponent final : public dae::Component {
public:
    explicit GameDirectorComponent(dae::GameObject& owner) : dae::Component(owner) {}
    void Update() override {
        // Since gamemodes aren't a gameobject they don't get updated so we force the update through a GameDirector Component
        GameModeManager::GetInstance().Update();
    }
    void Render() const override {}
};
