// Copy constructor of a layered class with a refcounted pointer at +0x88.
#include "types.h"

extern void* g_unk_7101a22d48[];
extern void* g_unk_7101a24060[];
extern void* g_unk_7101a3e3a8[];
extern void* g_unk_7101a3e8b8[];

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 ref_count;
    };
};

void fn_71004c9eec(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a3e3a8[2];
    *reinterpret_cast<u64*>(self + 0x28) = *reinterpret_cast<const u64*>(other + 0x28);
    *reinterpret_cast<u32*>(self + 0x30) = *reinterpret_cast<const u32*>(other + 0x30);
    *reinterpret_cast<u32*>(self + 0x34) = *reinterpret_cast<const u32*>(other + 0x34);
    *reinterpret_cast<u32*>(self + 0x38) = *reinterpret_cast<const u32*>(other + 0x38);
    *reinterpret_cast<u32*>(self + 0x3c) = *reinterpret_cast<const u32*>(other + 0x3c);
    *reinterpret_cast<u32*>(self + 0x40) = *reinterpret_cast<const u32*>(other + 0x40);
    *reinterpret_cast<u32*>(self + 0x44) = *reinterpret_cast<const u32*>(other + 0x44);
    *reinterpret_cast<u32*>(self + 0x48) = *reinterpret_cast<const u32*>(other + 0x48);
    *reinterpret_cast<u32*>(self + 0x4c) = *reinterpret_cast<const u32*>(other + 0x4c);
    *reinterpret_cast<u32*>(self + 0x50) = *reinterpret_cast<const u32*>(other + 0x50);
    *reinterpret_cast<u32*>(self + 0x54) = *reinterpret_cast<const u32*>(other + 0x54);
    __builtin_memcpy(self + 0x58, other + 0x58, 9);
    *reinterpret_cast<void***>(self) = &g_unk_7101a3e8b8[2];
    *reinterpret_cast<u64*>(self + 0x68) = *reinterpret_cast<const u64*>(other + 0x68);
    *reinterpret_cast<void***>(self + 0x70) = &g_unk_7101a22d48[2];
    *reinterpret_cast<u64*>(self + 0x78) = *reinterpret_cast<const u64*>(other + 0x78);
    *reinterpret_cast<u64*>(self + 0x80) = *reinterpret_cast<const u64*>(other + 0x80);
    *reinterpret_cast<u64*>(self + 0x88) = 0;
    UnkRefObj* n = *reinterpret_cast<UnkRefObj* const*>(other + 0x88);
    if (n) {
        ++n->ref_count;
        UnkRefObj* old = *reinterpret_cast<UnkRefObj**>(self + 0x88);
        if (old) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        *reinterpret_cast<UnkRefObj**>(self + 0x88) = n;
    }
}
