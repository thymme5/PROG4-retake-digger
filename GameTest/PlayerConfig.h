#pragma once
enum class InputType 
{ 
    Keyboard, 
    Gamepad 
};
struct PlayerConfig
{
    InputType inputType;
    int controllerIndex = 0;
};
