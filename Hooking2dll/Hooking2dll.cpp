// Hooking2dll.cpp : Defines the exported functions for the DLL application.
//

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

// EasyHook will be looking for this export to support DLL injection. If not found then 
// DLL injection will fail.
extern "C" void __declspec(dllexport) __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo);

void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{
    std::cout << "\n\nNativeInjectionEntryPointt(REMOTE_ENTRY_INFO* inRemoteInfo)\n\n" <<
        "IIIII           jjj               tt                dd !!! \n"
        " III  nn nnn          eee    cccc tt      eee       dd !!! \n"
        " III  nnn  nn   jjj ee   e cc     tttt  ee   e  dddddd !!! \n"
        " III  nn   nn   jjj eeeee  cc     tt    eeeee  dd   dd     \n"
        "IIIII nn   nn   jjj  eeeee  ccccc  tttt  eeeee  dddddd !!! \n"
        "              jjjj                                         \n\n";

    Hook("GetClientRect", itcpGetClientRect);
    Hook("CreateWindowExW", itcpCreateWindowExW);

    RhWakeUpProcess();

    return;
}