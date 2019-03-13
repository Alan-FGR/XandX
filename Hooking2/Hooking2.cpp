#include "pch.h"
#include <iostream>

#include <string>

#include <chrono>

#include "cxxopts.hpp"

#include <easyhook.h>
#if _WIN64
#pragma comment(lib, "EasyHook64.lib")
#else
#pragma comment(lib, "EasyHook32.lib")
#endif

#include "HookParams.h"

void pl(const char* l){std::cout << l << std::endl;}

std::tuple<int16_t, int16_t> parseSize(std::string xSepSize)
{
    auto x = xSepSize.substr(0, xSepSize.find("x"));
    auto y = xSepSize.substr(xSepSize.find("x")+1, xSepSize.length());
    std::cout << "Parsed size: " << xSepSize << std::endl;
    return std::make_tuple(std::stoi(x), std::stoi(y));
}


using namespace std;
int main(int argc, char* argv[])
{
    HookParams hpars;
    static PVOID addr = (PVOID)&hpars;

    WCHAR* binWChars;

    try {
        cxxopts::Options options("cc");

        options.add_options()
            ("s,windowSize", "Internal Window Size", cxxopts::value<std::string>())
            ("p,windowPos", "Internal Window Position", cxxopts::value<std::string>())
            ("w,forceWindow", "Force Windowed Mode (TODO)")//TODO
            ("b,borderless", "Borderless Window")
            ;

        if (argc < 3)
        {
            pl("Usage:");
            pl("  Arguments:");
            pl("    cc.exe client_size [OPTIONS] program");
            pl("  Options:");
            auto optHelp = options.help();
            cout << optHelp.substr(optHelp.find("]")+3, optHelp.length());
            pl("NOTE: All sizes are in the format WIDTHxHEIGHT, example: 1280x720\n");
            pl("Examples:");
            pl("  Run with custom client size of 720x480:");
            pl("    cc.exe 720x480 SomeProgram.exe");
            pl("  Run with custom client size and place borderless 800x600 window at 100x200");
            pl("    cc.exe 720x480 -s 800x600 -p 100x200 -b SomeProgram.exe");
            exit(-1);
        }

        int pargc = argc - 2;
        char** taddr = &argv[1];

        pl("Parsing arguments...");
        auto pars = options.parse(pargc, taddr);

        if (pars.count("s")) {
            pl("Found window size option, parsing size...");
            auto[x, y] = parseSize(pars["s"].as<std::string>());
            hpars.windowWidth = x;
            hpars.windowHeight = y;
        }

        if (pars.count("p")) {
            pl("Found window position option, parsing size...");
            auto[x, y] = parseSize(pars["p"].as<std::string>());
            hpars.windowPosX = x;
            hpars.windowPosY = y;
        }

        if (pars.count("b"))
        {
            pl("Found borderless flag.");
            hpars.borderLess = true;
        }

        pl("Parsing client size...");
        std::tuple<int16_t, int16_t> clientSize = parseSize(argv[1]);
        const char* binChars = argv[argc - 1];

        pl("Parsing program name...");
        size_t bcSize;
        mbsrtowcs_s(&bcSize, nullptr, 0, &binChars, 0, nullptr);
        binWChars = new WCHAR[bcSize + 1]();
        mbsrtowcs_s(&bcSize, binWChars, bcSize + 1, &binChars, bcSize + 1, nullptr);
    }
    catch (...) { pl("ERROR: Couldn't parse options, refer to usage by running without arguments."); exit(-1); }

    PROCESS_INFORMATION pi;
    STARTUPINFO si;

    memset(&si, 0, sizeof STARTUPINFO);
    memset(&pi, 0, sizeof PROCESS_INFORMATION);

    //auto hr = CreateProcess(binWChars, nullptr, nullptr, nullptr, true, 0, nullptr, nullptr, &si, &pi);

    const WCHAR* dllToInject = L"C:/Projects/RetroX/x64/Release/Hooking2dll.dll";
    wprintf(L"Attempting to inject: %s\n\n", dllToInject);

    ULONG pid;

    NTSTATUS nt = RhCreateAndInject(binWChars, nullptr, CREATE_SUSPENDED, EASYHOOK_INJECT_DEFAULT,
#if _WIN64
        nullptr,
        (WCHAR*)dllToInject,
#else
        (WCHAR*)dllToInject,
        nullptr,
#endif
        addr, sizeof HookParams, &pid);

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
        printf("Injection failed with code = %d\n", nt);
        std::wcout << RtlGetLastErrorString() << "\n";
    }

    // std::wcout << "Press Enter to exit";
    // std::wstring input;
    // std::getline(std::wcin, input);

	// HANDLE pHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, pid);
	//
	// WaitForSingleObject(pHandle, INFINITE);

    return 0;
}