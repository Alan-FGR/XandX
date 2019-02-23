// RetroX.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <cstdio>

#include <windows.h>
#include "detours.h"

#define SDL_MAIN_HANDLED
#include "SDL.h"

void fatal(const char* err)
{
    printf(err);
    printf(SDL_GetError());
    SDL_Quit();
    exit(-1);
}

int main()
{
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) fatal("SDL couldn't initialize");
    
    auto win = SDL_CreateWindow("RetroX", 10,10,400,300,0);

    if (win == nullptr) fatal("Couldn't create window");

    auto rdr = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);





    while (1)
    {
        SDL_Event evt;
        SDL_PollEvent(&evt);


        SDL_SetRenderDrawColor(rdr, 255, 255, 255, 255);
        SDL_RenderClear(rdr);



        SDL_RenderPresent(rdr);

    }



    printf("SDL meh");

    return 0;
}
