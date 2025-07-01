#include "ColorMap.h"

std::unordered_map<std::string, SDL_Rect> g_ColorSpriteMap = {
    { "purple",             SDL_Rect{  0,  0, 32, 32 } },
    { "yellow_orange",      SDL_Rect{ 32,  0, 32, 32 } },
    { "light_gray",         SDL_Rect{ 64,  0, 32, 32 } },
    { "blue",               SDL_Rect{ 96,  0, 32, 32 } },
    { "yellow_black",       SDL_Rect{128,  0, 32, 32 } },
    { "light_blue",         SDL_Rect{160,  0, 32, 32 } },

    { "yellow_green",       SDL_Rect{  0, 32, 32, 32 } },
    { "blue_brown",         SDL_Rect{ 32, 32, 32, 32 } },
    { "orange",             SDL_Rect{ 64, 32, 32, 32 } },
    { "dark_gray",          SDL_Rect{ 96, 32, 32, 32 } },
    { "lime",               SDL_Rect{128, 32, 32, 32 } },
    { "navy",               SDL_Rect{160, 32, 32, 32 } },

    { "red",                SDL_Rect{  0, 64, 32, 32 } },
    { "green",              SDL_Rect{ 32, 64, 32, 32 } },
    { "blue_gray",          SDL_Rect{ 64, 64, 32, 32 } },
    { "magenta",            SDL_Rect{ 96, 64, 32, 32 } },
    { "pink",               SDL_Rect{128, 64, 32, 32 } },
    { "gold",               SDL_Rect{160, 64, 32, 32 } }
};
