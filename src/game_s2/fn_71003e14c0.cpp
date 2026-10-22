// Copy constructor of a layered class with a refcounted pointer at +0x50.
#include "types.h"

extern void* g_unk_7101a22d48[];
extern void* g_unk_7101a24060[];
extern void* g_unk_7101a2fa38[];

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 ref_count;
    };
};

void fn_71003e14c0(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a2fa38[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 16);
    *reinterpret_cast<void***>(self + 0x38) = &g_unk_7101a22d48[2];
    *reinterpret_cast<u64*>(self + 0x40) = *reinterpret_cast<const u64*>(other + 0x40);
    *reinterpret_cast<u64*>(self + 0x48) = *reinterpret_cast<const u64*>(other + 0x48);
    *reinterpret_cast<u64*>(self + 0x50) = 0;
    UnkRefObj* n = *reinterpret_cast<UnkRefObj* const*>(other + 0x50);
    if (n) {
        ++n->ref_count;
        UnkRefObj* old = *reinterpret_cast<UnkRefObj**>(self + 0x50);
        if (old) {
            if (--old->ref_count == 0)
                old->vf_0x10();
        }
        *reinterpret_cast<UnkRefObj**>(self + 0x50) = n;
    }
    *reinterpret_cast<u8*>(self + 0x58) = *reinterpret_cast<const u8*>(other + 0x58);
}
