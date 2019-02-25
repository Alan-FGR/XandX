// Hooking2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include <string>

#include <easyhook.h>
#include <minwinbase.h>
#include <chrono>

#if _WIN64
#pragma comment(lib, "EasyHook64.lib")
#else
#pragma comment(lib, "EasyHook32.lib")
#endif

using namespace std;
int main(int argc, char* argv[])
{
    const char* binChars = argv[1];

    size_t resultSize;
    mbsrtowcs_s(&resultSize, nullptr, 0, &binChars, 0, nullptr);

    WCHAR* binWChars = new WCHAR[resultSize+1]();

    mbsrtowcs_s(&resultSize, binWChars, resultSize + 1, &binChars, resultSize + 1, nullptr);

    PROCESS_INFORMATION pi;
    STARTUPINFO si;

    ZeroMemory(&si, sizeof STARTUPINFO);
    ZeroMemory(&pi, sizeof PROCESS_INFORMATION);

    //auto hr = CreateProcess(binWChars, nullptr, nullptr, nullptr, true, 0, nullptr, nullptr, &si, &pi);

    const WCHAR* dllToInject = L"C:/Projects/RetroX/x64/Release/Hooking2dll.dll";
    wprintf(L"Attempting to inject: %s\n\n", dllToInject);

    HMODULE hmod = GetModuleHandle(0);

    static PVOID addr = (PVOID)&hmod;

    ULONG outPID;
    NTSTATUS nt = RhCreateAndInject(binWChars, nullptr, CREATE_SUSPENDED, EASYHOOK_INJECT_DEFAULT, nullptr,
        (WCHAR*)dllToInject, nullptr, 0, &outPID);

    // NTSTATUS nt = RhInjectLibrary(
    //     pi.dwProcessId,   // The process to inject into
    //     0,           // ThreadId to wake up upon injection
    //     EASYHOOK_INJECT_DEFAULT,
    //     NULL, // 32-bit
    //     (WCHAR*)dllToInject,		 // 64-bit not provided
    //     &addr, // data to send to injected DLL entry point
    //     sizeof (HMODULE)// size of data to send
    // );


    if (nt != 0)
    {
        printf("RhInjectLibrary failed with error code = %d\n", nt);
        PWCHAR err = RtlGetLastErrorString();
        std::wcout << err << "\n";
    }
    else
    {
        std::wcout << L"Library injected successfully.\n";
    }


    std::wcout << "Press Enter to exit";
    std::wstring input;
    std::getline(std::wcin, input);
    std::getline(std::wcin, input);
    return 0;
}