#pragma once
#include "pch.h"

enum class HookMode : uint8_t
{
	Reapply, // reapplies changes when window is shown, works for most programs that support windows manipulation
	Modify // simply modifies the calls and feeds altered data back into the program
};

struct HookParams
{
    uint16_t fakeClientWidth{ 800 };
    uint16_t fakeClientHeight{ 600 };
    int16_t windowHeight{ -1 };
    int16_t windowWidth{ -1 };
    int16_t windowPosX{ -1 };
    int16_t windowPosY{ -1 };
    bool borderLess{ false };
	HookMode mode{ HookMode::Reapply };
};
