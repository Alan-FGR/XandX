// Hooking2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <easyhook.h>

#if _WIN64
#pragma comment(lib, "EasyHook64.lib")
#else
#pragma comment(lib, "EasyHook32.lib")
#endif

using namespace std;

BOOL WINAPI myBeepHook(DWORD dwFreq, DWORD dwDuration);

BOOL WINAPI myBeepHook(DWORD dwFreq, DWORD dwDuration)
{
    cout << "\n****All your beeps belong to us!\n\n";
    return Beep(dwFreq + 800, dwDuration);
}

int main(int argc, TCHAR* argv[])
{
    HOOK_TRACE_INFO hHook = { NULL }; // keep track of our hook
    cout << "\n";
    cout << GetProcAddress(GetModuleHandle(TEXT("kernel32")), "Beep");

    // Install the hook
    NTSTATUS result = LhInstallHook(
        GetProcAddress(GetModuleHandle(TEXT("kernel32")), "Beep"),
        myBeepHook,
        NULL,
        &hHook);
    if (FAILED(result))
    {
        wstring s(RtlGetLastErrorString());
        wcout << "Failed to install hook: ";
        wcout << s.c_str();
        cout << "\n\nPress any key to exit.";
        cin.get();
        return -1;
    }

    cout << "Beep after hook installed but not enabled.\n";
    Beep(500, 500);

    cout << "Activating hook for current thread only.\n";
    // If the threadId in the ACL is set to 0, 
    // then internally EasyHook uses GetCurrentThreadId()
    ULONG ACLEntries[1] = { 0 };
    LhSetInclusiveACL(ACLEntries, 1, &hHook);

    cout << "Beep after hook enabled.\n";
    Beep(500, 500);

    cout << "Uninstall hook\n";
    LhUninstallHook(&hHook);

    cout << "Beep after hook uninstalled\n";
    Beep(500, 500);

    cout << "\n\nRestore ALL entry points of pending removals issued by LhUninstallHook()\n";
    LhWaitForPendingRemovals();

    cout << "Press any key to exit.";
    cin.get();

    return 0;
}