#pragma once
#include "GameObject.h"

class InputBindingHelper
{
public:
    static void BindPlayer1KeyboardInputs(dae::GameObject* qbert);
    static void BindPlayer1GamepadInputs(dae::GameObject* qbert);
    static void BindPlayer2KeyboardInputs(dae::GameObject* qbert);
    static void BindPlayer2GamepadInputs(dae::GameObject* qbert);
    static void BindPlayer2CoilyGamepadInputs(dae::GameObject* coily);
};
