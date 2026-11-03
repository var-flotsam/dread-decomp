// A unit whose only code is its static initialiser: the guarded initialiser of an inline variable
// holding four angles in degrees (45, 135, 315, 225). Its guard and object are reached through the
// GOT.
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkAngles4 {
    float v[4];
};

extern u64 g_unk_717270ae98;
extern UnkAngles4 g_unk_717270ae88;

void fn_71006e0f30() {
    if (!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_717270ae98), __ATOMIC_ACQUIRE) & 1)) {
        if (__cxa_guard_acquire(&g_unk_717270ae98)) {
            g_unk_717270ae88 = {{45.0f, 135.0f, 315.0f, 225.0f}};
            __cxa_guard_release(&g_unk_717270ae98);
            asm volatile("");  // the release is a call, not a tail call
        }
    }
}
