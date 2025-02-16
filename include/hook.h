#pragma once

#include "endstone/level/level.h"
#include <cstdio>

#include <string>
#include <fstream>

#if defined(__GNUC__) 
    void *getAddr(){
        std::string addressRange;
        std::ifstream mapsFile("/proc/self/maps");

        if (mapsFile.is_open()) {
            std::string line;
            std::getline(mapsFile, line);

            size_t start = line.find("-");
            size_t end = line.find(" ");

            addressRange = line.substr(0, start);

            mapsFile.close();
        } else {
            std::printf("Failed to open /proc/self/maps");
        }
        return (ssize_t)std::stol(addressRange, NULL, 16);
    }
#else
	#include <windows.h>
	#include <Psapi.h>

	   void* getAddr(){
	       // Get the current process ID
	       DWORD currentProcessId = GetCurrentProcessId();

	       // Get a handle to the current process
	       HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, currentProcessId);

	       // Get the base address of the DLL
	       HMODULE hModule = NULL;
	       GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetModuleHandle(NULL), &hModule);

	       // Get the executable path
	       TCHAR szPath[MAX_PATH];
	       GetModuleFileName(hModule, szPath, MAX_PATH);

	       // Get the process image file name
	       char szProcessPath[MAX_PATH];
	       GetProcAddress(hModule, szProcessPath);

	       // Get the module information
	       MODULEINFO mi;
	       GetModuleInformation(hProcess, hModule, &mi, sizeof(mi));

	       // Close the process handle
	       CloseHandle(hProcess);

	       return mi.lpBaseOfDll;

	   }

#endif 

int install_hooks(void *startingAddress);
static void (*original_tick_func)();
static void tick_hook();

namespace Tick {
    static bool tickFreeze = false;
    static int tickCounter = 0;
    static int tickSlowdown = 1;
    static int tickAccel = 1;
    static int stepCounter = 0;
}
