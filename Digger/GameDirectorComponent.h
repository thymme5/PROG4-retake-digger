#include "Component.h"
#include "GameObject.h"
#include "GameModeManager.h"

class GameDirectorComponent final : public dae::Component {
public:
    explicit GameDirectorComponent(dae::GameObject& owner) : dae::Component(owner) {}
    void Update() override {
        GameModeManager::GetInstance().Update();
    }
    void Render() const override {}
};
