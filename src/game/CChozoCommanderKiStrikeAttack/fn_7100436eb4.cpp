// Copy constructor of a layered class.
#include "types.h"

extern void* g_unk_7101a24060[];
extern void* g_unk_7101a35248[];
extern void* g_unk_7101a356c8[];
extern void* g_unk_7101a35d30[];

struct UnkRefObj {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        u32 ref_count;
    };
};

void fn_7100436eb4(char* self, const char* other) {
    *reinterpret_cast<void***>(self) = &g_unk_7101a24060[2];
    __builtin_memcpy(self + 0x8, other + 0x8, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a35248[2];
    __builtin_memcpy(self + 0x28, other + 0x28, 32);
    *reinterpret_cast<void***>(self) = &g_unk_7101a356c8[2];
    __builtin_memcpy(self + 0x48, other + 0x48, 8);
    *reinterpret_cast<void***>(self) = &g_unk_7101a35d30[2];
    __builtin_memcpy(self + 0x50, other + 0x50, 28);
    __builtin_memcpy(self + 0x6c, other + 0x6c, 4);
    __builtin_memcpy(self + 0x70, other + 0x70, 4);
    __builtin_memcpy(self + 0x74, other + 0x74, 4);
    __builtin_memcpy(self + 0x78, other + 0x78, 4);
    __builtin_memcpy(self + 0x7c, other + 0x7c, 4);
    __builtin_memcpy(self + 0x80, other + 0x80, 4);
    __builtin_memcpy(self + 0x84, other + 0x84, 1);
}
