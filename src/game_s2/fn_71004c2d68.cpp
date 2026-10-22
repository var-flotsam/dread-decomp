// Copy constructor of a three-level class whose last level also holds a refcounted pointer.
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a3e3a8[];
extern void* g_unk_7101a3e4b0[];
extern void* g_unk_7101a22d48[];

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 ref_count;
    };
};

void fn_71004c2d68(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 8, other + 8, 0x20);
    *reinterpret_cast<void***>(self) = &g_unk_7101a3e3a8[2];
    *reinterpret_cast<u64*>(self + 0x28) = *reinterpret_cast<const u64*>(other + 0x28);
    for (u32 off = 0x30; off <= 0x54; off += 4)
        *reinterpret_cast<u32*>(self + off) = *reinterpret_cast<const u32*>(other + off);
    __builtin_memcpy(self + 0x58, other + 0x58, 9);
    *reinterpret_cast<void***>(self) = &g_unk_7101a3e4b0[2];
    __builtin_memcpy(self + 0x68, other + 0x68, 13);
    *reinterpret_cast<void***>(self + 0x78) = &g_unk_7101a22d48[2];
    *reinterpret_cast<u64*>(self + 0x80) = *reinterpret_cast<const u64*>(other + 0x80);
    *reinterpret_cast<u64*>(self + 0x88) = *reinterpret_cast<const u64*>(other + 0x88);
    *reinterpret_cast<UnkRefObj**>(self + 0x90) = nullptr;
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
