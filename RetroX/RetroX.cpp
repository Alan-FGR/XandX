// RetroX.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <cstdio>

#define SDL_MAIN_HANDLED
#include "SDL.h"

void fatal(const char* err)
{
    printf(err);
    printf(SDL_GetError());
    exit(-1);
}

int main()
{
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) fatal("SDL couldn't initialize");
    
    auto win = SDL_CreateWindow("RetroX", 0,0,400,300,SDL_WINDOW_BORDERLESS);

    if (win == nullptr) fatal("Couldn't create window");



    printf("SDL meh");

    return 0;
}
