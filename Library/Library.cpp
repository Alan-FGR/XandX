#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) { return TRUE; }

#include <easyhook.h>
#if _WIN64
#pragma comment(lib, "EasyHook64.lib")
#else
#pragma comment(lib, "EasyHook32.lib")
#endif

#include "HookParams.h"

HookParams params;

#define HP(x,o) (o == -1 ? x : o)
#define IfBL(t,f) (params.borderLess ? t : f)

#if _DEBUG
FILE* logfile = NULL;
void log(char const* const c, ...)
{
	printf(c);
	if (logfile == NULL)
		fopen_s(&logfile, "XandX_log.txt", "a");
	fprintf_s(logfile, c);
}
#define printf(x) log(x)
#endif

void UpdateWindowRect(LPRECT& lpRect)
{
	if (params.windowPosX >= 0) {
		lpRect->left = params.windowPosX;
		lpRect->top  = params.windowPosY;
	}
	if (params.mode == HookMode::Modify && params.windowWidth >= 0) {
		lpRect->right = lpRect->left + params.windowWidth;
		lpRect->bottom = lpRect->top + params.windowHeight;
	}
}

HWND WINAPI itcpCreateWindowExW(_In_ DWORD dwExStyle, _In_opt_ LPCWSTR lpClassName, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) {
#if _DEBUG
	printf("INTERCEPTED CreateWindowExW\n");
#endif
	HWND r;
	if (params.mode == HookMode::Modify)
		r = CreateWindowExW(dwExStyle, lpClassName, lpWindowName,
				            IfBL(WS_POPUP, dwStyle),
				            HP(X, params.windowPosX), HP(Y, params.windowPosY),
				            HP(nWidth, params.windowWidth), HP(nHeight, params.windowHeight),
				            hWndParent, hMenu, hInstance, lpParam);
	else
		r = CreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle,
		                    HP(X, params.windowPosX), HP(Y, params.windowPosY),
		                    nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	return r;
}

HWND WINAPI itcpCreateWindowExA(_In_ DWORD dwExStyle, _In_opt_ LPCSTR lpClassName, _In_opt_ LPCSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam) {
#if _DEBUG
	printf("INTERCEPTED CreateWindowExA\n");
#endif
	HWND r;
	if (params.mode == HookMode::Modify)
		r = CreateWindowExA(dwExStyle, lpClassName, lpWindowName,
				            IfBL(WS_POPUP, dwStyle),
				            HP(X, params.windowPosX), HP(Y, params.windowPosY),
				            HP(nWidth, params.windowWidth), HP(nHeight, params.windowHeight),
				            hWndParent, hMenu, hInstance, lpParam);
	else
		r = CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle,
		                    HP(X, params.windowPosX), HP(Y, params.windowPosY),
		                    nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	return r;
}

BOOL WINAPI itcpMoveWindow(_In_ HWND hWnd, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_ BOOL bRepaint) {
#if _DEBUG
	printf("INTERCEPTED MoveWindow\n");
#endif
	BOOL r;
	if (params.mode == HookMode::Modify)
		r = MoveWindow(hWnd,
		               HP(X, params.windowPosX), HP(Y, params.windowPosY), HP(nWidth, params.windowWidth),
		               HP(nHeight, params.windowHeight),
		               bRepaint);
	else
		r = MoveWindow(hWnd, HP(X, params.windowPosX), HP(Y, params.windowPosY), nWidth, nHeight, bRepaint);
	return r;
}

BOOL WINAPI itcpSetWindowPos(_In_ HWND hWnd, _In_opt_ HWND hWndInsertAfter, _In_ int X, _In_ int Y, _In_ int cx, _In_ int cy, _In_ UINT uFlags) {
#if _DEBUG
	printf("INTERCEPTED SetWindowPos\n");
#endif
	BOOL r;
	if (params.mode == HookMode::Modify)
		r = SetWindowPos(hWnd, hWndInsertAfter,
		HP(X, params.windowPosX), HP(Y, params.windowPosY), HP(cx, params.windowWidth), HP(cy, params.windowHeight),
		uFlags);
	else
		r = SetWindowPos(hWnd, hWndInsertAfter, HP(X, params.windowPosX), HP(Y, params.windowPosY), cx, cy, uFlags);
	return r;
}

BOOL WINAPI itcpGetClientRect(_In_ HWND hWnd, _Out_ LPRECT lpRect) {
#if _DEBUG
	printf("INTERCEPTED GetClientRect\n");
#endif
	BOOL r = GetClientRect(hWnd, lpRect);
	{
		lpRect->right = lpRect->left + params.fakeClientWidth;
		lpRect->bottom = lpRect->top + params.fakeClientHeight;
	}
	return r;
}

BOOL WINAPI itcpAdjustWindowRect(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu) {
#if _DEBUG
	printf("INTERCEPTED AdjustWindowRect\n");
#endif
	auto r = AdjustWindowRect(lpRect, dwStyle, bMenu);
	UpdateWindowRect(lpRect);
	return r;
}

BOOL WINAPI itcpAdjustWindowRectEx(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu, _In_ DWORD dwExStyle) {
#if _DEBUG
	printf("INTERCEPTED AdjustWindowRectEx\n");
#endif
	auto r = AdjustWindowRectEx(lpRect, dwStyle, bMenu, dwExStyle);
	UpdateWindowRect(lpRect);
	return r;
}

BOOL WINAPI itcpShowWindow(_In_ HWND hWnd, _In_ int nCmdShow) {
#if _DEBUG
	printf("INTERCEPTED ShowWindow\n");
#endif
	auto r = ShowWindow(hWnd, nCmdShow);

	if  (  nCmdShow == SW_MAXIMIZE       
		|| nCmdShow == SW_RESTORE        
		|| nCmdShow == SW_SHOW           
		|| nCmdShow == SW_SHOWDEFAULT    
		|| nCmdShow == SW_SHOWMAXIMIZED  
		|| nCmdShow == SW_SHOWNA         
		|| nCmdShow == SW_SHOWNOACTIVATE 
		|| nCmdShow == SW_SHOWNORMAL
		)
	{
		if (params.mode == HookMode::Reapply) {
			if (params.borderLess)
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
			SetWindowPos(hWnd, HWND_TOP,
				HP(0, params.windowPosX), HP(0, params.windowPosY),
				HP(params.fakeClientWidth, params.windowWidth), HP(params.fakeClientHeight, params.windowHeight),
				SWP_NOMOVE | SWP_NOREPOSITION | SWP_SHOWWINDOW);
		}
	}

	return r; //TODO
}



#if _DEBUG

BOOL WINAPI itcpGetWindowRect(_In_ HWND hWnd, _Out_ LPRECT lpRect) {
#if _DEBUG
	printf("INTERCEPTED GetWindowRect\n");
#endif
	auto r = GetWindowRect(hWnd, lpRect);
	return r; //TODO
}

#if(WINVER >= 0x0605)
BOOL WINAPI itcpAdjustWindowRectExForDpi(_Inout_ LPRECT lpRect, _In_ DWORD dwStyle, _In_ BOOL bMenu, _In_ DWORD dwExStyle, _In_ UINT dpi) {
#if _DEBUG
	printf("INTERCEPTED AdjustWindowRectExForDpi\n");
#endif
	auto r = AdjustWindowRectExForDpi(lpRect, dwStyle, bMenu, dwExStyle, dpi);
	return r; //TODO
}
#endif

BOOL WINAPI itcpGetWindowPlacement(_In_ HWND hWnd, _Inout_ WINDOWPLACEMENT* lpwndpl) {
#if _DEBUG
	printf("INTERCEPTED GetWindowPlacement\n");
#endif
	auto r = GetWindowPlacement(hWnd, lpwndpl);
	// if (params.mode == HookMode::Modify && params.windowWidth >= 0)
	// {
	// 	lpwndpl->rcNormalPosition.right = lpwndpl->rcNormalPosition.left + params.windowWidth*2;
	// 	lpwndpl->rcNormalPosition.bottom = lpwndpl->rcNormalPosition.top + params.windowHeight*2;
	// 	lpwndpl->ptMaxPosition.x = lpwndpl->ptMinPosition.x + params.windowWidth*2;
	// 	lpwndpl->ptMaxPosition.y = lpwndpl->ptMinPosition.y + params.windowHeight*2;
	// }
	return r; //TODO
}

BOOL WINAPI itcpSetWindowPlacement(_In_ HWND hWnd, _In_ CONST WINDOWPLACEMENT* lpwndpl) {
#if _DEBUG
	printf("INTERCEPTED SetWindowPlacement\n");
#endif
	auto r = SetWindowPlacement(hWnd, lpwndpl);
	return r; //TODO
}

BOOL WINAPI itcpClientToScreen(_In_ HWND hWnd, _Inout_ LPPOINT lpPoint) {
#if _DEBUG
	printf("INTERCEPTED ClientToScreen\n");
#endif
	auto r = ClientToScreen(hWnd, lpPoint);
	return r; //TODO
}

BOOL WINAPI itcpScreenToClient(_In_ HWND hWnd, _Inout_ LPPOINT lpPoint) {
#if _DEBUG
	printf("INTERCEPTED ScreenToClient\n");
#endif
	auto r = ScreenToClient(hWnd, lpPoint);
	return r; //TODO
}

#endif



void browse()
{
	// UpdateWindow;
	// SetWindowLongA;
	// GetWindowLongA;
	// SetWindowLongW;
	// GetWindowLongW;
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
		std::wcout << s << std::endl;
	}
	else
	{
		std::wcout << "Hook installed successfully: ";
		std::wcout << funcName << std::endl;
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

    params = *reinterpret_cast<HookParams*>(inRemoteInfo->UserData);

	std::vector<HOOK_TRACE_INFO> hooks;

	hooks.emplace_back(Hook("CreateWindowExW", itcpCreateWindowExW));
	hooks.emplace_back(Hook("CreateWindowExA", itcpCreateWindowExA));
	hooks.emplace_back(Hook("MoveWindow", itcpMoveWindow));
	hooks.emplace_back(Hook("SetWindowPos", itcpSetWindowPos));
	hooks.emplace_back(Hook("GetClientRect", itcpGetClientRect));
	hooks.emplace_back(Hook("MoveWindow", itcpMoveWindow));
	hooks.emplace_back(Hook("SetWindowPos", itcpSetWindowPos));
	hooks.emplace_back(Hook("AdjustWindowRect", itcpAdjustWindowRect));
	hooks.emplace_back(Hook("AdjustWindowRectEx", itcpAdjustWindowRectEx));
	hooks.emplace_back(Hook("ShowWindow", itcpShowWindow));

 #if _DEBUG
 	hooks.emplace_back(Hook("GetWindowRect", itcpGetWindowRect));
 #if(WINVER >= 0x0605)
	hooks.emplace_back(Hook("AdjustWindowRectExForDpi", itcpAdjustWindowRectExForDpi));
 #endif
 	hooks.emplace_back(Hook("GetWindowPlacement", itcpGetWindowPlacement));
	hooks.emplace_back(Hook("SetWindowPlacement", itcpSetWindowPlacement));
	hooks.emplace_back(Hook("ClientToScreen", itcpClientToScreen));
 	hooks.emplace_back(Hook("ScreenToClient", itcpScreenToClient));
 #endif

	
    RhWakeUpProcess();

	//WaitForSingleObject(OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, inRemoteInfo->HostPID), INFINITE);

}