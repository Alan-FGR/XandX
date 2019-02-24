// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "detours.h"

#include "SDL.h"
#include <cstdio>
#include <vector>
#include <string>
#include <chrono>

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

static std::wstring curLog;
void log(std::string msg, bool pause = false)
{
    printf(msg.c_str());
    printf("\n");
    return;
    //msg = std::to_wstring(((uint64_t)
    //    std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count())
    //    %10000000000ul)
    //    + L": " + msg + L"\r\n";
    //curLog = msg + curLog;

    //if (curLog.size() > 1000)
    //    curLog.resize(1000);

    //TCHAR *pbuf = (TCHAR*)&(curLog.data()[0]);
    //SetWindowText(textArea, pbuf);
    ////todo pause (for dbg)
}

HWND(WINAPI *origCreateWindowExA)(_In_ DWORD dwExStyle, _In_opt_ LPCSTR lpClassName, _In_opt_ LPCSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) = CreateWindowExA;
HWND WINAPI itcpCreateWindowExA(_In_ DWORD dwExStyle, _In_opt_ LPCSTR lpClassName, _In_opt_ LPCSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) {
    auto r = origCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    log("itcpCreateWindowExA");
    return r;
}
void hookCreateWindowExA() { DetourAttach(&(PVOID&)origCreateWindowExA, itcpCreateWindowExA); }
void detachCreateWindowExA() { DetourDetach(&(PVOID&)origCreateWindowExA, itcpCreateWindowExA); }

HWND(WINAPI *origCreateWindowExW)(_In_ DWORD dwExStyle, _In_opt_ LPCWSTR lpClassName, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) = CreateWindowExW;
HWND WINAPI itcpCreateWindowExW(_In_ DWORD dwExStyle, _In_opt_ LPCWSTR lpClassName, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) {
    //auto r = origCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    auto r = origCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, 440, 0, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    log("itcpCreateWindowExW");
    return  r;
}
void hookCreateWindowExW() { DetourAttach(&(PVOID&)origCreateWindowExW, itcpCreateWindowExW); }
void detachCreateWindowExW() { DetourDetach(&(PVOID&)origCreateWindowExW, itcpCreateWindowExW); }

BOOL(WINAPI *origGetClientRect)(_In_ HWND hWnd, _Out_ LPRECT lpRect) = GetClientRect;
BOOL WINAPI itcpGetClientRect(_In_ HWND hWnd, _Out_ LPRECT lpRect) {
    auto r = origGetClientRect(hWnd, lpRect);
    lpRect->right = 200;
    //lpRect->bottom = lpRect->top + 300;
    log("itcpGetClientRect");
    return r;
}
void hookGetClientRect() { DetourAttach(&(PVOID&)origGetClientRect, itcpGetClientRect); }
void detachGetClientRect() { DetourDetach(&(PVOID&)origGetClientRect, itcpGetClientRect); }

BOOL(WINAPI *origGetWindowRect)(_In_ HWND hWnd, _Out_ LPRECT lpRect) = GetWindowRect;
BOOL WINAPI itcpGetWindowRect(_In_ HWND hWnd, _Out_ LPRECT lpRect) {
    auto r = origGetWindowRect(hWnd, lpRect);
    log("itcpGetWindowRect");
    return r;
}
void hookGetWindowRect() { DetourAttach(&(PVOID&)origGetWindowRect, itcpGetWindowRect); }
void detachGetWindowRect() { DetourDetach(&(PVOID&)origGetWindowRect, itcpGetWindowRect); }

BOOL(WINAPI *origAdjustWindowRectEx)(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu, _In_ DWORD dwExStyle) = AdjustWindowRectEx;
BOOL WINAPI itcpAdjustWindowRectEx(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu, _In_ DWORD dwExStyle) {
    log("hooked itcpAdjustWindowRectEx called");
    auto r = origAdjustWindowRectEx(lpRect, dwStyle, bMenu, dwExStyle);
    return r;
}
void hookAdjustWindowRectEx() { DetourAttach(&(PVOID&)origAdjustWindowRectEx, itcpAdjustWindowRectEx); }
void detachAdjustWindowRectEx() { DetourDetach(&(PVOID&)origAdjustWindowRectEx, itcpAdjustWindowRectEx); }


BOOL(WINAPI *origAdjustWindowRect)(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu) = AdjustWindowRect;
BOOL WINAPI itcpAdjustWindowRect(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu) {
    log("hooked itcpAdjustWindowRect called");
    auto r = origAdjustWindowRect(lpRect, dwStyle, bMenu);
    return r;
}
void hookAdjustWindowRect() { DetourAttach(&(PVOID&)origAdjustWindowRect, itcpAdjustWindowRect); }
void detachAdjustWindowRect() { DetourDetach(&(PVOID&)origAdjustWindowRect, itcpAdjustWindowRect); }


//BOOL(WINAPI *origAdjustWindowRectExForDpi)(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu, _In_ DWORD dwExStyle, _In_ UINT dpi) = AdjustWindowRectExForDpi;
//BOOL WINAPI itcpAdjustWindowRectExForDpi(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu, _In_ DWORD dwExStyle, _In_ UINT dpi) {
//    log("hooked itcpAdjustWindowRectExForDpi called");
//    auto r = origAdjustWindowRectExForDpi(lpRect, dwStyle, bMenu, dwExStyle, dpi);
//    return r;
//}
//void hookAdjustWindowRectExForDpi() { DetourAttach(&(PVOID&)origAdjustWindowRectExForDpi, itcpAdjustWindowRectExForDpi); }
//void detachAdjustWindowRectExForDpi() { DetourDetach(&(PVOID&)origAdjustWindowRectExForDpi, itcpAdjustWindowRectExForDpi); }

HWND(WINAPI *origCreateMDIWindowW)(_In_ LPCWSTR lpClassName, _In_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HINSTANCE hInstance, _In_ LPARAM lParam) = CreateMDIWindowW;
HWND WINAPI itcpCreateMDIWindowW(_In_ LPCWSTR lpClassName, _In_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HINSTANCE hInstance, _In_ LPARAM lParam) {
    log("hooked itcpCreateMDIWindowW called");
    auto r = origCreateMDIWindowW(lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hInstance, lParam);
    return r;
}
void hookCreateMDIWindowW() { DetourAttach(&(PVOID&)origCreateMDIWindowW, itcpCreateMDIWindowW); }
void detachCreateMDIWindowW() { DetourDetach(&(PVOID&)origCreateMDIWindowW, itcpCreateMDIWindowW); }


HWND(WINAPI *origCreateMDIWindowA)(_In_ LPCSTR lpClassName, _In_ LPCSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HINSTANCE hInstance, _In_ LPARAM lParam) = CreateMDIWindowA;
HWND WINAPI itcpCreateMDIWindowA(_In_ LPCSTR lpClassName, _In_ LPCSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HINSTANCE hInstance, _In_ LPARAM lParam) {
    log("hooked itcpCreateMDIWindowA called");
    auto r = origCreateMDIWindowA(lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hInstance, lParam);
    return r;
}
void hookCreateMDIWindowA() { DetourAttach(&(PVOID&)origCreateMDIWindowA, itcpCreateMDIWindowA); }
void detachCreateMDIWindowA() { DetourDetach(&(PVOID&)origCreateMDIWindowA, itcpCreateMDIWindowA); }

BOOL(WINAPI *origShowWindow)(_In_ HWND hWnd, _In_ int nCmdShow) = ShowWindow;
BOOL WINAPI itcpShowWindow(_In_ HWND hWnd, _In_ int nCmdShow) {
    log("hooked itcpShowWindow called");
    auto r = origShowWindow(hWnd, nCmdShow);
    return r;
}
void hookShowWindow() { DetourAttach(&(PVOID&)origShowWindow, itcpShowWindow); }
void detachShowWindow() { DetourDetach(&(PVOID&)origShowWindow, itcpShowWindow); }

BOOL(WINAPI *origShowWindowAsync)(_In_ HWND hWnd, _In_ int nCmdShow) = ShowWindowAsync;
BOOL WINAPI itcpShowWindowAsync(_In_ HWND hWnd, _In_ int nCmdShow) {
    log("hooked itcpShowWindowAsync called");
    auto r = origShowWindowAsync(hWnd, nCmdShow);
    return r;//TODO
}
void hookShowWindowAsync() { DetourAttach(&(PVOID&)origShowWindowAsync, itcpShowWindowAsync); }
void detachShowWindowAsync() { DetourDetach(&(PVOID&)origShowWindowAsync, itcpShowWindowAsync); }

















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

        //createLogger(hModule);

        DetourRestoreAfterWith();
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        //attach stuff
        hookGetClientRect();
        hookCreateWindowExA();
        hookCreateWindowExW();
        hookGetWindowRect();
        hookAdjustWindowRect();
        hookAdjustWindowRectEx();
        //hookAdjustWindowRectExForDpi();
        hookCreateMDIWindowA();
        hookCreateMDIWindowW();
        hookShowWindow();
        hookShowWindowAsync();

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

