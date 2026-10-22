// Guarded initialiser of the global id "spinjump2attack_dashslash" (an inline variable): constructs it
// once and registers its destructor to run at exit.
#include "game/UnkTUInit.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

extern u64 g_unk_71726cbba0;
extern u8 g_unk_71726cbb98[];

void fn_7100467e70() {
    if (!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726cbba0), __ATOMIC_ACQUIRE) & 1)) {
        if (__cxa_guard_acquire(&g_unk_71726cbba0)) {
            new (g_unk_71726cbb98) CStrId("spinjump2attack_dashslash", true);
            fn_7100000250(fn_710000051c, g_unk_71726cbb98, &g_unk_7101a19000);
            __cxa_guard_release(&g_unk_71726cbba0);
        }
    }
}
