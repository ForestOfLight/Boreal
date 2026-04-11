#pragma once
#include <cstdint>
#include <windows.h>

class MemoryPatch {
    void *address;
    uint8_t original;
    uint8_t patched;
    bool applied = false;

    bool write(uint8_t value) {
        DWORD oldProtect;
        if (!VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &oldProtect))
            return false;
        *reinterpret_cast<uint8_t *>(address) = value;
        VirtualProtect(address, 1, oldProtect, &oldProtect);
        applied = (value == patched);
        return true;
    }

public:
    MemoryPatch(void *address, uint8_t patchValue)
        : address(address)
        , original(*reinterpret_cast<uint8_t *>(address))
        , patched(patchValue) {}

    bool apply() {
        return write(patched);
    }

    bool restore() {
        return write(original);
    }

    void setValue(uint8_t newValue) {
        patched = newValue;
        if (applied)
            write(patched);
    }

    bool isApplied()    const { return applied; }
    void *getAddress()  const { return address; }
    uint8_t getOriginal() const { return original; }
};
