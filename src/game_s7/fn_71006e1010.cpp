// Copies of fn_7100898e80 (src/game_s7/fn_71005b9d70.cpp) for other types, with their own callees and data (the names, strings and comments below are the original's): fn_71006e1010, fn_71006e5a90.
// Guarded initialisers of global ids (inline variables), copies of fn_7100464520: each constructs
// its id once and registers its destructor to run at exit.
#include "game/UnkTUInit.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

extern u64 g_unk_71726eb3d8;
extern u8 g_unk_71726eb3d0[];
extern u64 g_unk_717270aea8;
extern u8 g_unk_717270aea0[];
extern u64 g_unk_717272d618;
extern u8 g_unk_717272d610[];

void fn_71006e1010() {
    if (!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_717272d618), __ATOMIC_ACQUIRE) & 1)) {
        if (__cxa_guard_acquire(&g_unk_717272d618)) {
            new (g_unk_717272d610) CStrId("finished", true);
            fn_7100000250(fn_710000051c, g_unk_717272d610, &g_unk_7101a19000);
            __cxa_guard_release(&g_unk_717272d618);
        }
    }
}

void fn_71006e5a90() {
    if (!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_717272d618), __ATOMIC_ACQUIRE) & 1)) {
        if (__cxa_guard_acquire(&g_unk_717272d618)) {
            new (g_unk_717272d610) CStrId("finished", true);
            fn_7100000250(fn_710000051c, g_unk_717272d610, &g_unk_7101a19000);
            __cxa_guard_release(&g_unk_717272d618);
        }
    }
}

