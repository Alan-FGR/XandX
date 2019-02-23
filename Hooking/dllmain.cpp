// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "detours.h"

#include "SDL.h"
#include <cstdio>

__declspec(dllexport) void magic() {}

static int(*origSDL_RenderClear)(SDL_Renderer* renderer) = SDL_RenderClear;

static BOOL(WINAPI *origGetClientRect)(_In_ HWND hWnd, _Out_ LPRECT lpRect) = GetClientRect;

BOOL iGetClientRect(_In_ HWND hWnd, _Out_ LPRECT lpRect)
{
    auto r = origGetClientRect(hWnd, lpRect);
    lpRect->right = lpRect->left+400;
    lpRect->bottom = lpRect->top+300;
    return r;
}

int iClear(SDL_Renderer* renderer)
{
    printf("sux\n");
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    auto r = origSDL_RenderClear(renderer);
    //SDL_RenderClear(renderer);
    return r;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        printf("attaching");
        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        //DetourAttach(&(PVOID&)origSDL_RenderClear, iClear);
        DetourAttach(&(PVOID&)origGetClientRect, iGetClientRect);
        DetourTransactionCommit();
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        //DetourDetach(&(PVOID&)origSDL_RenderClear, iClear);
        DetourTransactionCommit();
    }
    return TRUE;
}

