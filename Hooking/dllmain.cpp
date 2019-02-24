// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "detours.h"

#include "SDL.h"
#include <cstdio>

__declspec(dllexport) void magic() {} // :notes: I never did beliee-ee-ee-eeve in the ways of magic... :trollface:

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

static HWND textArea;
void createLogger(HINSTANCE hInstance)
{

    auto className = L"Hook Logger";
    
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = className;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    HWND hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        className,
        className,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    textArea = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"LOG START\n",
        WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOHSCROLL, 0, 0, 400, 380, hwnd, NULL, NULL, NULL);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
}

void log(const char* msg, bool pause = false)
{
    int index = GetWindowTextLength(textArea);
    SetFocus(textArea);
    SendMessageA(textArea, EM_SETSEL, (WPARAM)index, (LPARAM)index);
    SendMessageA(textArea, EM_REPLACESEL, 0, (LPARAM)msg);
}


HWND(WINAPI *origCreateWindowExA)(_In_ DWORD dwExStyle, _In_opt_ LPCSTR lpClassName, _In_opt_ LPCSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) = CreateWindowExA;
HWND WINAPI itcpCreateWindowExA(_In_ DWORD dwExStyle, _In_opt_ LPCSTR lpClassName, _In_opt_ LPCSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) {
    auto r = origCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    return r;
}
void hookCreateWindowExA() { DetourAttach(&(PVOID&)origCreateWindowExA, itcpCreateWindowExA); }
void detachCreateWindowExA() { DetourDetach(&(PVOID&)origCreateWindowExA, itcpCreateWindowExA); }

HWND(WINAPI *origCreateWindowExW)(_In_ DWORD dwExStyle, _In_opt_ LPCWSTR lpClassName, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) = CreateWindowExW;
HWND WINAPI itcpCreateWindowExW(_In_ DWORD dwExStyle, _In_opt_ LPCWSTR lpClassName, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) {
    auto r = origCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    return  r;
}
void hookCreateWindowExW() { DetourAttach(&(PVOID&)origCreateWindowExW, itcpCreateWindowExW); }
void detachCreateWindowExW() { DetourDetach(&(PVOID&)origCreateWindowExW, itcpCreateWindowExW); }

BOOL(WINAPI *origGetClientRect)(_In_ HWND hWnd, _Out_ LPRECT lpRect) = GetClientRect;
BOOL WINAPI itcpGetClientRect(_In_ HWND hWnd, _Out_ LPRECT lpRect) {
    auto r = origGetClientRect(hWnd, lpRect);
    lpRect->right = lpRect->left + 400;
    lpRect->bottom = lpRect->top + 300;
    return r;
}
void hookGetClientRect() { DetourAttach(&(PVOID&)origGetClientRect, itcpGetClientRect); }
void detachGetClientRect() { DetourDetach(&(PVOID&)origGetClientRect, itcpGetClientRect); }

BOOL(WINAPI *origGetWindowRect)(_In_ HWND hWnd, _Out_ LPRECT lpRect) = GetWindowRect;
BOOL WINAPI itcpGetWindowRect(_In_ HWND hWnd, _Out_ LPRECT lpRect) {
    auto r = origGetWindowRect(hWnd, lpRect);
    return r;
}
void hookGetWindowRect() { DetourAttach(&(PVOID&)origGetWindowRect, itcpGetWindowRect); }
void detachGetWindowRect() { DetourDetach(&(PVOID&)origGetWindowRect, itcpGetWindowRect); }







void browse()
{

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  dwReason, LPVOID lpReserved)
{
    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH) {
        printf("attaching");

        createLogger(hModule);

        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        //attach stuff
        hookGetClientRect();
        hookCreateWindowExA();
        hookCreateWindowExW();
        hookGetWindowRect();

        DetourTransactionCommit();
    }
    else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        //detach stuff
        detachGetClientRect();

        DetourTransactionCommit();
    }
    return TRUE;
}

