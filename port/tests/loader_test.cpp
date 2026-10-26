// The data loader's test library: runtime/src/loader.cpp and this file, built without a C runtime
// into a DLL (a shared object elsewhere) by tools/test_port_loader.py, which loads it with ctypes
// and runs the loader on the user's dump. Everything the test needs from inside the process is
// here: the host's memory services (runtime/src/host_memory.h), two functions for the function
// table to point at, recorders standing in for the runtime's traps, and the copies the compiler may
// call for. Built with -fno-builtin, so the loops below stay loops.

#include "dread_port_loader.h"
#include "host_memory.h"

#if defined(_WIN32)
#define TEST_EXPORT extern "C" __declspec(dllexport)
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void* address, unsigned long long size, unsigned long type);
#else
#define TEST_EXPORT extern "C" __attribute__((visibility("default")))
#endif

using size_type = decltype(sizeof(0));

extern "C" {

void* memcpy(void* to, const void* from, size_type n) {
    auto* t = static_cast<u8*>(to);
    const auto* f = static_cast<const u8*>(from);
    for (size_type i = 0; i < n; ++i)
        t[i] = f[i];
    return to;
}

void* memmove(void* to, const void* from, size_type n) {
    auto* t = static_cast<u8*>(to);
    const auto* f = static_cast<const u8*>(from);
    if (t < f) {
        for (size_type i = 0; i < n; ++i)
            t[i] = f[i];
    } else {
        for (size_type i = n; i > 0; --i)
            t[i - 1] = f[i - 1];
    }
    return to;
}

void* memset(void* to, int value, size_type n) {
    auto* t = static_cast<u8*>(to);
    for (size_type i = 0; i < n; ++i)
        t[i] = u8(value);
    return to;
}

int memcmp(const void* a, const void* b, size_type n) {
    const auto* x = static_cast<const u8*>(a);
    const auto* y = static_cast<const u8*>(b);
    for (size_type i = 0; i < n; ++i)
        if (x[i] != y[i])
            return x[i] < y[i] ? -1 : 1;
    return 0;
}

}  // extern "C"

namespace {

u64 g_missing_function;
const char* g_missing_import;
u64 g_missing_calls;

}  // namespace

// Where the function table points: distinct, recognisable results.
TEST_EXPORT u64 test_code_a(u64 x) { return x + 1; }
TEST_EXPORT u64 test_code_b(u64 x) { return x * 2; }

// The traps, returning instead of aborting so that the test can call a thunk and look.
TEST_EXPORT void test_record_missing_function(u64 address) {
    g_missing_function = address;
    ++g_missing_calls;
}

TEST_EXPORT void test_record_missing_import(const char* name) {
    g_missing_import = name;
    ++g_missing_calls;
}

TEST_EXPORT u64 test_last_missing_function() { return g_missing_function; }
TEST_EXPORT const char* test_last_missing_import() { return g_missing_import; }
TEST_EXPORT u64 test_missing_calls() { return g_missing_calls; }

TEST_EXPORT s32 test_load(const u8* nso, u64 size, const DreadPortLoadOptions* options, DreadPortImageInfo* info) {
    const DreadPortHost host = dread_port_host_memory::Host(nullptr);
    return dread_port_load_image(nso, size, options, &host, info);
}

TEST_EXPORT const char* test_status_text(s32 status) {
    return dread_port_load_status_text(DreadPortLoadStatus(status));
}

// Gives the image's range back, so that a test process can load twice.
TEST_EXPORT bool test_unmap(u64 address, u64 size) {
#if defined(_WIN32)
    (void)size;
    return VirtualFree(reinterpret_cast<void*>(address), 0, 0x8000 /* MEM_RELEASE */) != 0;
#else
    return munmap(reinterpret_cast<void*>(address), size) == 0;
#endif
}
