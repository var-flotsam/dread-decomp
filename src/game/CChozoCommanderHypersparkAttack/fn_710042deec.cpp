// Copy constructor of a layered class with a refcounted pointer at +0x90.
#include "types.h"

extern void* g_unk_7101a22d48[];
extern void* g_unk_7101a24060[];
extern void* g_unk_7101a35248[];
extern void* g_unk_7101a356c8[];
extern void* g_unk_7101a358f8[];

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 ref_count;
    };
};

void fn_710042deec(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a35248[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a356c8[2];
    __builtin_memcpy(self + 0x48, other + 0x48, 8);
    *reinterpret_cast<void***>(self) = &g_unk_7101a358f8[2];
    __builtin_memcpy(self + 0x50, other + 0x50, 36);
    *reinterpret_cast<void***>(self + 0x78) = &g_unk_7101a22d48[2];
    __builtin_memcpy(self + 0x80, other + 0x80, 8);
    __builtin_memcpy(self + 0x88, other + 0x88, 8);
    *reinterpret_cast<u64*>(self + 0x90) = 0;
    UnkRefObj* n = *reinterpret_cast<UnkRefObj* const*>(other + 0x90);
    if (n) {
        ++n->ref_count;
        UnkRefObj* old = *reinterpret_cast<UnkRefObj**>(self + 0x90);
        if (old) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        *reinterpret_cast<UnkRefObj**>(self + 0x90) = n;
    }
}
