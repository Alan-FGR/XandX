
#include "stdafx.h"

#include <easyhook.h>
#if _WIN64
#pragma comment(lib, "EasyHook64.lib")
#else
#pragma comment(lib, "EasyHook32.lib")
#endif

#include <string>
#include <iostream>
#include <Windows.h>

#include "Logging.h"

#include "../Hooking2/HookParams.h"

void Hook(const char* funcName, void* calledFunc)
{
    HOOK_TRACE_INFO hHook = { NULL }; // keep track of our hook

    // Install the hook
    NTSTATUS result = LhInstallHook(GetProcAddress(GetModuleHandle(TEXT("user32")), funcName),
        calledFunc, NULL, &hHook);

    if (FAILED(result))
    {
        std::wstring s(RtlGetLastErrorString());
        std::wcout << "Failed to install hook: ";
        std::wcout << s;
    }
    else
    {
        std::wcout << "Hook installed successfully: ";
        std::wcout << funcName;
    }

    // If the threadId in the ACL is set to 0,
    // then internally EasyHook uses GetCurrentThreadId()
    ULONG ACLEntries[1] = { 0 };

    // Disable the hook for the provided threadIds, enable for all others
    LhSetExclusiveACL(ACLEntries, 1, &hHook);
}

HWND WINAPI itcpCreateWindowExW(_In_ DWORD dwExStyle, _In_opt_ LPCWSTR lpClassName, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) {
    //auto r = origCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    auto r = CreateWindowExW(dwExStyle, lpClassName, L"HOOKED", WS_POPUP, 440, 220, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    printf("\n\nitcpCreateWindowExW\n\n");
    return  r;
}

BOOL WINAPI itcpGetClientRect(_In_ HWND hWnd, _Out_ LPRECT lpRect) {
    auto r = GetClientRect(hWnd, lpRect);
    lpRect->right = 200;
    //lpRect->bottom = lpRect->top + 300;
    //printf("itcpGetClientRect");
    return r;
}

void browse()
{
    UpdateWindow       ;
    MoveWindow         ;
    SetWindowPos       ;
    CreateWindowExA    ;
    CreateWindowExW    ;
                       ;
    SetWindowLongA     ;
    GetWindowLongA     ;
    SetWindowLongW     ;
    GetWindowLongW     ;
                       ;
    AdjustWindowRect   ;
    AdjustWindowRectEx ;
    ShowWindow         ;
                       ;
    ScreenToClient     ;
    ClientToScreen     ;
    GetClientRect      ;
    GetWindowRect      ;
}

extern "C" __declspec(dllexport) void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{
    std::cout << "\nINJECTION ENTRY POINT REACHED!\n";

    HookParams hpars = *reinterpret_cast<HookParams*>(inRemoteInfo->UserData);

    std::cout << hpars.fakeClientWidth << std::endl;

    Hook("GetClientRect", itcpGetClientRect);
    Hook("CreateWindowExW", itcpCreateWindowExW);

    RhWakeUpProcess();

    return;
}