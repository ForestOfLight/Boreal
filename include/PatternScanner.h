#pragma once

#ifdef _WIN32
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
  #include <windows.h>
  #include <Psapi.h>
#elif defined(__linux__)
  #include <fstream>
  #include <string>
#endif

class PatternScanner {
public:
    struct Match {
        void* address;
    };

    // Scan a known memory range.
    static std::vector<Match> scan(const void* start, size_t searchSize, const uint8_t* pattern, size_t patternSize) {
        std::vector<Match> matches;
        auto* base = reinterpret_cast<const uint8_t*>(start);
        auto* searchPtr = base;
        auto* end = base + searchSize;

        while (searchPtr < end) {
            if (remainingSize(searchPtr, end) < patternSize)
                break;

            if (matchAt(searchPtr, pattern, patternSize)) {
                addMatch(matches, searchPtr);
                searchPtr += patternSize;
            } else {
                ++searchPtr;
            }
        }

        return matches;
    }

    // Scan the entire module image that contains moduleBase.
    static std::vector<Match> scan(const void* moduleBase, const uint8_t* pattern, size_t patternSize) {
        return scan(moduleBase, getModuleSize(moduleBase), pattern, patternSize);
    }

private:
    static size_t getModuleSize(const void* base) {
#ifdef _WIN32
        MODULEINFO mi{};
        GetModuleInformation(GetCurrentProcess(),
                             reinterpret_cast<HMODULE>(const_cast<void*>(base)),
                             &mi, sizeof(mi));
        return mi.SizeOfImage;
#elif defined(__linux__)
        // Walk /proc/self/maps to find the contiguous extent of mappings from base.
        // Stop at the first gap — that marks the end of the module image.
        const auto baseAddr = reinterpret_cast<uintptr_t>(base);
        uintptr_t prevEnd = baseAddr;

        std::ifstream maps("/proc/self/maps");
        std::string line;
        while (std::getline(maps, line)) {
            uintptr_t start, end;
            if (sscanf(line.c_str(), "%lx-%lx", &start, &end) != 2)
                continue;
            if (start < baseAddr) continue;
            if (start > prevEnd)  break;  // gap = end of module
            prevEnd = end;
        }
        return prevEnd - baseAddr;
#else
        return 0;
#endif
    }

    static size_t remainingSize(const uint8_t* ptr, const uint8_t* end) {
        return static_cast<size_t>(end - ptr);
    }

    static bool matchAt(const uint8_t* ptr, const uint8_t* pattern, size_t patternSize) {
        for (size_t i = 0; i < patternSize; ++i) {
            if (ptr[i] != pattern[i])
                return false;
        }
        return true;
    }

    static void addMatch(std::vector<Match>& matches, const uint8_t* ptr) {
        matches.push_back({ const_cast<uint8_t*>(ptr) });
    }
};
