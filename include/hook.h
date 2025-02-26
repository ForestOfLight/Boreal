#pragma once

#include <funchook.h>
#include "Tick.h"

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
        return (void *)std::stol(addressRange, NULL, 16);
    }
#else
	#define NOMINMAX
	#include <windows.h>
	#include <Psapi.h>

	   void* getBaseAddress(){
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

int install_hooks(void *baseAddress)
{
    int rv;

    funchook_set_debug_file("funchook-debug");
    funchook_t *funchook = funchook_create();

    TickSpeed::hook(baseAddress, funchook);

    /* Install hooks.
	 * The first 5-byte code of tick() and recv() are changed respectively.
	 */
    rv = funchook_install(funchook, 0);
    if (rv != 0) {
    	/* error */
    }
    return rv;
}
