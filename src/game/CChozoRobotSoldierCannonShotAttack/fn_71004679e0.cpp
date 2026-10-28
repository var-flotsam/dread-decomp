// Copy constructor of a layered class with a refcounted pointer at +0x60.
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a383a0[];

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 ref_count;
    };
};

void fn_71004679e0(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a383a0[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 37);
    *reinterpret_cast<u32*>(self + 0x50) = *reinterpret_cast<const u32*>(other + 0x50);
    *reinterpret_cast<u32*>(self + 0x54) = *reinterpret_cast<const u32*>(other + 0x54);
    *reinterpret_cast<u32*>(self + 0x58) = *reinterpret_cast<const u32*>(other + 0x58);
    *reinterpret_cast<u32*>(self + 0x5c) = *reinterpret_cast<const u32*>(other + 0x5c);
    *reinterpret_cast<u64*>(self + 0x60) = 0;
    UnkRefObj* n = *reinterpret_cast<UnkRefObj* const*>(other + 0x60);
    if (n) {
        ++n->ref_count;
        UnkRefObj* old = *reinterpret_cast<UnkRefObj**>(self + 0x60);
        if (old) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        *reinterpret_cast<UnkRefObj**>(self + 0x60) = n;
    }
    *reinterpret_cast<u64*>(self + 0x68) = *reinterpret_cast<const u64*>(other + 0x68);
}
