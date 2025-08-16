#pragma once
#include "Component.h"
#include "TextComponent.h"
#include <array>

class NameEntryComponent final : public dae::Component
{
public:
    NameEntryComponent(dae::GameObject& owner);

    void ChangeLetter(int direction); // +1 or -1
    void MoveCursor(int direction);   // +1 or -1
    void Confirm();

    void Update() override;
    void Render() const override {}

private:
    std::array<char, 3> m_Initials{ 'A', 'A', 'A' };
    int m_CursorIndex{ 0 };
    dae::TextComponent* m_Text{ nullptr };

    void RefreshDisplay();
};
