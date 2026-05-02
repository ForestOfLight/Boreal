#pragma once

#include <funchook.h>

#include "classes/NativePlayerCache.h"
#include "classes/ForceOpenContainers.h"
#include "classes/Tick.h"
#include "classes/PlayersTickLevelChunks.h"
#include "classes/PistonPushLimit.h"
#include "classes/PlayerAbilities.h"

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

	   static MODULEINFO getModuleInfo() {
	       HMODULE hModule = NULL;
	       GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GetModuleHandle(NULL), &hModule);
	       HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());
	       MODULEINFO mi{};
	       GetModuleInformation(hProcess, hModule, &mi, sizeof(mi));
	       CloseHandle(hProcess);
	       return mi;
	   }

	   void* getBaseAddress(){
	       return getModuleInfo().lpBaseOfDll;
	   }

#endif

int install_hooks(void *baseAddress)
{
    int rv;

    funchook_set_debug_file("funchook-debug");
    funchook_t *funchook = funchook_create();

    TickSpeed::hook(baseAddress, funchook);
    PlayersTickLevelChunks::hook(baseAddress, funchook);
    PistonPushLimit::install(baseAddress);
    ForceOpenContainers::hook(baseAddress, funchook);
    PlayerAbilities::install(baseAddress);

    /* Install hooks.
	 * The first 5-byte code of tick() and recv() are changed respectively.
	 */
    rv = funchook_install(funchook, 0);
    if (rv != 0) {
    	/* error */
    }
    return rv;
}
