#pragma once

// The memory services the data loader needs (DreadPortHost in dread_port_loader.h), over the
// host's virtual memory: a fixed-address mapping for the game's image, read-only protection, and
// executable memory for the trap thunks. Shared by the runtime (image.cpp) and the loader's test
// library (tools/test_port_loader.py), which is built without a C runtime: on Windows this
// declares the four kernel32 functions it uses itself instead of including <windows.h>, so it
// needs no Windows SDK.

#include "dread_port_loader.h"

#if defined(_WIN32)

extern "C" {
__declspec(dllimport) void* __stdcall VirtualAlloc(void* address, unsigned long long size, unsigned long type,
                                                   unsigned long protect);
__declspec(dllimport) int __stdcall VirtualProtect(void* address, unsigned long long size, unsigned long protect,
                                                   unsigned long* old_protect);
__declspec(dllimport) void* __stdcall GetCurrentProcess();
__declspec(dllimport) int __stdcall FlushInstructionCache(void* process, const void* address, unsigned long long size);
}

namespace dread_port_host_memory {

constexpr unsigned long kMemCommit = 0x1000, kMemReserve = 0x2000;
constexpr unsigned long kPageReadOnly = 0x02, kPageReadWrite = 0x04, kPageExecuteRead = 0x20,
                        kPageExecuteReadWrite = 0x40;

// The whole range is committed at once: the .bss is 1.82 GB of commit charge, backed by the page
// file until touched, which the console's reservation is too.
inline void* MapFixed(u64 address, u64 size) {
    return VirtualAlloc(reinterpret_cast<void*>(address), size, kMemReserve | kMemCommit, kPageReadWrite);
}

inline bool ProtectReadOnly(void* address, u64 size) {
    unsigned long old;
    return VirtualProtect(address, size, kPageReadOnly, &old) != 0;
}

inline void* MapExecutable(u64 size) {
    return VirtualAlloc(nullptr, size, kMemReserve | kMemCommit, kPageReadWrite);
}

inline bool SealExecutable(void* address, u64 size) {
    unsigned long old;
    return VirtualProtect(address, size, kPageExecuteRead, &old) != 0 &&
           FlushInstructionCache(GetCurrentProcess(), address, size) != 0;
}

}  // namespace dread_port_host_memory

#else

#include <sys/mman.h>

namespace dread_port_host_memory {

// MAP_FIXED would replace whatever is mapped there; a hint and a check never does. Anonymous
// memory is zero-filled and reserved lazily on Linux and macOS.
inline void* MapFixed(u64 address, u64 size) {
    void* want = reinterpret_cast<void*>(address);
    void* got = mmap(want, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    if (got == MAP_FAILED)
        return nullptr;
    if (got != want) {
        munmap(got, size);
        return nullptr;
    }
    return got;
}

inline bool ProtectReadOnly(void* address, u64 size) { return mprotect(address, size, PROT_READ) == 0; }

inline void* MapExecutable(u64 size) {
    void* got = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return got == MAP_FAILED ? nullptr : got;
}

inline bool SealExecutable(void* address, u64 size) { return mprotect(address, size, PROT_READ | PROT_EXEC) == 0; }

}  // namespace dread_port_host_memory

#endif

namespace dread_port_host_memory {

inline DreadPortHost Host(void (*log)(const char*)) {
    return DreadPortHost{MapFixed, ProtectReadOnly, MapExecutable, SealExecutable, log};
}

}  // namespace dread_port_host_memory
