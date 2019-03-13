
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
#include <vector>

HookParams params;

#define HP(x,o) (o == -1 ? x : o)
#define IfBL(t,f) (params.borderLess ? t : f)

HWND WINAPI itcpCreateWindowExW(_In_ DWORD dwExStyle, _In_opt_ LPCWSTR lpClassName, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) {
#if _DEBUG
	printf("INTERCEPTED CreateWindowExW\n");
#endif
	auto r = CreateWindowExW(dwExStyle, lpClassName, lpWindowName, IfBL(WS_POPUP, dwStyle),
		HP(X, params.windowPosX), HP(Y, params.windowPosY), HP(nWidth, params.windowWidth), HP(nHeight, params.windowHeight),
		hWndParent, hMenu, hInstance, lpParam);
	return r;
}

HWND WINAPI itcpCreateWindowExA(_In_ DWORD dwExStyle, _In_opt_ LPCSTR lpClassName, _In_opt_ LPCSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) {
#if _DEBUG
	printf("INTERCEPTED CreateWindowExA\n");
#endif
	auto r = CreateWindowExA(dwExStyle, lpClassName, lpWindowName, IfBL(WS_POPUP, dwStyle),
		HP(X, params.windowPosX), HP(Y, params.windowPosY), HP(nWidth, params.windowWidth), HP(nHeight, params.windowHeight),
		hWndParent, hMenu, hInstance, lpParam);
	return r;
}

BOOL WINAPI itcpMoveWindow(_In_ HWND hWnd, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_ BOOL bRepaint) {
#if _DEBUG
	printf("INTERCEPTED MoveWindow\n");
#endif
	auto r = MoveWindow(hWnd,
		HP(X, params.windowPosX), HP(Y, params.windowPosY), HP(nWidth, params.windowWidth), HP(nHeight, params.windowHeight),
		bRepaint);
	return r;
}

BOOL WINAPI itcpSetWindowPos(_In_ HWND hWnd, _In_opt_ HWND hWndInsertAfter, _In_ int X, _In_ int Y, _In_ int cx, _In_ int cy, _In_ UINT uFlags) {
#if _DEBUG
	printf("INTERCEPTED SetWindowPos\n");
#endif
	auto r = SetWindowPos(hWnd, hWndInsertAfter,
		HP(X, params.windowPosX), HP(Y, params.windowPosY), HP(cx, params.windowWidth), HP(cy, params.windowHeight),
		uFlags);
	return r;
}

BOOL WINAPI itcpGetClientRect(_In_ HWND hWnd, _Out_ LPRECT lpRect) {
#if _DEBUG
	printf("INTERCEPTED GetClientRect\n");
#endif
	auto r = GetClientRect(hWnd, lpRect);

	return r; TODO
}


BOOL WINAPI itcpGetWindowRect(_In_ HWND hWnd, _Out_ LPRECT lpRect) {
#if _DEBUG
	printf("INTERCEPTED GetWindowRect\n");
#endif
	auto r = GetWindowRect(hWnd, lpRect);
	return r; TODO
}


BOOL WINAPI itcpAdjustWindowRect(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu) {
#if _DEBUG
	printf("INTERCEPTED AdjustWindowRect\n");
#endif
	auto r = AdjustWindowRect(lpRect, dwStyle, bMenu);
	return r; TODO
}

BOOL WINAPI itcpAdjustWindowRectEx(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu, _In_ DWORD dwExStyle) {
#if _DEBUG
	printf("INTERCEPTED AdjustWindowRectEx\n");
#endif
	auto r = AdjustWindowRectEx(lpRect, dwStyle, bMenu, dwExStyle);
	return r; TODO
}

#if(WINVER >= 0x0605)
BOOL WINAPI itcpAdjustWindowRectExForDpi(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu, _In_ DWORD dwExStyle, _In_ UINT dpi) {
#if _DEBUG
	printf("INTERCEPTED AdjustWindowRectExForDpi\n");
#endif
	auto r = AdjustWindowRectExForDpi(lpRect, dwStyle, bMenu, dwExStyle, dpi);
	return r; TODO
}
#endif

BOOL WINAPI itcpMoveWindow(_In_ HWND hWnd, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_ BOOL bRepaint) {
#if _DEBUG
	printf("INTERCEPTED MoveWindow\n");
#endif
	auto r = MoveWindow(hWnd, X, Y, nWidth, nHeight, bRepaint);
	return r; TODO
}


BOOL WINAPI itcpSetWindowPos(_In_ HWND hWnd, _In_opt_ HWND hWndInsertAfter, _In_ int X, _In_ int Y, _In_ int cx, _In_ int cy, _In_ UINT uFlags) {
#if _DEBUG
	printf("INTERCEPTED SetWindowPos\n");
#endif
	auto r = SetWindowPos(hWnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
	return r; TODO
}


BOOL WINAPI itcpGetWindowPlacement(_In_ HWND hWnd, _Inout_ WINDOWPLACEMENT* lpwndpl) {
#if _DEBUG
	printf("INTERCEPTED GetWindowPlacement\n");
#endif
	auto r = GetWindowPlacement(hWnd, *lpwndpl);
	return r; TODO
}


BOOL WINAPI itcpSetWindowPlacement(_In_ HWND hWnd, _In_ CONST WINDOWPLACEMENT* lpwndpl) {
#if _DEBUG
	printf("INTERCEPTED SetWindowPlacement\n");
#endif
	auto r = SetWindowPlacement(hWnd, *lpwndpl);
	return r; TODO
}


BOOL WINAPI itcpShowWindow(_In_ HWND hWnd, _In_ int nCmdShow) {
#if _DEBUG
	printf("INTERCEPTED ShowWindow\n");
#endif
	auto r = ShowWindow(hWnd, nCmdShow);
	return r; TODO
}


BOOL WINAPI itcpClientToScreen(_In_ HWND hWnd, _Inout_ LPPOINT lpPoint) {
#if _DEBUG
	printf("INTERCEPTED ClientToScreen\n");
#endif
	auto r = ClientToScreen(hWnd, lpPoint);
	return r; TODO
}


BOOL WINAPI itcpScreenToClient(_In_ HWND hWnd, _Inout_ LPPOINT lpPoint) {
#if _DEBUG
	printf("INTERCEPTED ScreenToClient\n");
#endif
	auto r = ScreenToClient(hWnd, lpPoint);
	return r; TODO
}





void browse()
{
	UpdateWindow;
	SetWindowLongA;
	GetWindowLongA;
	SetWindowLongW;
	GetWindowLongW;
}

HOOK_TRACE_INFO Hook(const char* funcName, void* calledFunc)
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

	return hHook;
}

extern "C" __declspec(dllexport) void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* inRemoteInfo)
{
    std::cout << "\nINJECTION ENTRY POINT REACHED!\n";

    HookParams hpars = *reinterpret_cast<HookParams*>(inRemoteInfo->UserData);

    std::cout << hpars.fakeClientWidth << std::endl;

	std::vector<HOOK_TRACE_INFO> hooks;

	hooks.emplace_back(Hook("CreateWindowExW", itcpCreateWindowExW));
	hooks.emplace_back(Hook("CreateWindowExA", itcpCreateWindowExA));
	hooks.emplace_back(Hook("MoveWindow", itcpMoveWindow));
	hooks.emplace_back(Hook("SetWindowPos", itcpSetWindowPos));
	hooks.emplace_back(Hook("GetClientRect", itcpGetClientRect));
	hooks.emplace_back(Hook("GetWindowRect", itcpGetWindowRect));
	hooks.emplace_back(Hook("AdjustWindowRect", itcpAdjustWindowRect));
	hooks.emplace_back(Hook("AdjustWindowRectEx", itcpAdjustWindowRectEx));
	hooks.emplace_back(Hook("MoveWindow", itcpMoveWindow));
	hooks.emplace_back(Hook("SetWindowPos", itcpSetWindowPos));
	hooks.emplace_back(Hook("GetWindowPlacement", itcpGetWindowPlacement));
	hooks.emplace_back(Hook("SetWindowPlacement", itcpSetWindowPlacement));
	hooks.emplace_back(Hook("ShowWindow", itcpShowWindow));
	hooks.emplace_back(Hook("ClientToScreen", itcpClientToScreen));
	hooks.emplace_back(Hook("ScreenToClient", itcpScreenToClient));

#if(WINVER >= 0x0605)
	hooks.emplace_back(Hook("AdjustWindowRectExForDpi", itcpAdjustWindowRectExForDpi));
#endif

    RhWakeUpProcess();

	//WaitForSingleObject(OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, inRemoteInfo->HostPID), INFINITE);

}