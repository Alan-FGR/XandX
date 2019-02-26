#pragma once
#include "pch.h"

struct HookParams
{
    uint16_t fakeClientWidth{ 800 };
    uint16_t fakeClientHeight{ 600 };
    int16_t windowHeight{ -1 };
    int16_t windowWidth{ -1 };
    int16_t windowPosX{ -1 };
    int16_t windowPosY{ -1 };
    bool borderLess{ false };
};
