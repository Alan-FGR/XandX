// RetroX.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <cstdio>
#include <cmath>

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

    auto win = SDL_CreateWindow("RetroX", 10, 50, 400, 300, 0);

    if (win == nullptr) fatal("Couldn't create window");

    //SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    /*SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);*/

    auto rdr = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    SDL_RendererInfo inf;
    SDL_GetRendererInfo(rdr, &inf);
    SDL_SetWindowTitle(win, inf.name);

    auto bmp = SDL_LoadBMP("sample.bmp");
    auto tex = SDL_CreateTextureFromSurface(rdr, bmp);

    int pos = 0;

    while (1)
    {
        SDL_Event evt;
        SDL_PollEvent(&evt);

        int col = (pos/25)%2 == 0 ? 255 : 0;
        SDL_SetRenderDrawColor(rdr, col, col, col, 255);
        SDL_RenderClear(rdr);


        float nor = std::abs((pos%100)-50)/50.f;

        SDL_Rect rec{200*nor,150*nor,200,150};
        SDL_RenderCopy(rdr, tex, nullptr, &rec);

        SDL_RenderPresent(rdr);

        pos ++;

    }



    printf("SDL meh");

    return 0;
}
