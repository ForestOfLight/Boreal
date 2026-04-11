#pragma once

class PatternScanner {
public:
    struct Match {
        void* address;
    };

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

private:
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