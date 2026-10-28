// Copy constructor of a layered class.
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a42548[];

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 ref_count;
    };
};

void fn_71004ead00(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a42548[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 12);
    *reinterpret_cast<u32*>(self + 0x34) = *reinterpret_cast<const u32*>(other + 0x34);
    *reinterpret_cast<u32*>(self + 0x38) = *reinterpret_cast<const u32*>(other + 0x38);
    *reinterpret_cast<u32*>(self + 0x3c) = *reinterpret_cast<const u32*>(other + 0x3c);
    *reinterpret_cast<u32*>(self + 0x40) = *reinterpret_cast<const u32*>(other + 0x40);
    *reinterpret_cast<u32*>(self + 0x44) = *reinterpret_cast<const u32*>(other + 0x44);
    *reinterpret_cast<u32*>(self + 0x48) = *reinterpret_cast<const u32*>(other + 0x48);
    *reinterpret_cast<u32*>(self + 0x4c) = *reinterpret_cast<const u32*>(other + 0x4c);
    *reinterpret_cast<u32*>(self + 0x50) = *reinterpret_cast<const u32*>(other + 0x50);
    *reinterpret_cast<u32*>(self + 0x54) = *reinterpret_cast<const u32*>(other + 0x54);
    *reinterpret_cast<u32*>(self + 0x58) = *reinterpret_cast<const u32*>(other + 0x58);
    *reinterpret_cast<u32*>(self + 0x5c) = *reinterpret_cast<const u32*>(other + 0x5c);
    *reinterpret_cast<u32*>(self + 0x60) = *reinterpret_cast<const u32*>(other + 0x60);
    __builtin_memcpy(self + 0x64, other + 0x64, 5);
}
