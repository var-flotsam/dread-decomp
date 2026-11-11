// Unit 1117 of tools/tu_map.py (no class hint): the constructor of an object with a vtable
// (g_unk_7101a6b858) and a sub-object (+0xb0, g_unk_7101a59f90) after its base (fn_7100703814, which
// takes the id the constructor was given).
// Names and layouts are placeholders.
#include "game/UnkTU1124.h"
#include "types.h"

struct UnkUnit1124Object;

extern void* g_unk_7101a6b858[];
extern void* g_unk_7101a59f90[];

// The sub-object at +0xb0 (a vtable, then flags and words).
struct UnkSub1117_b0202 {
    void** vtable;
    u8 pad_0x8[0x4a];
    u8 field_0x52;
};

struct UnkObject1117_b0202 {
    void** vtable;                    // 0x0
    u8 pad_0x8[0xa8];
    UnkSub1117_b0202 sub;             // 0xb0
};

void fn_71006fbbfc(UnkObject1117_b0202* self, u32 id) {
    fn_7100703814(reinterpret_cast<UnkUnit1124Object*>(self), id);
    u8* p = reinterpret_cast<u8*>(self);
    self->vtable = g_unk_7101a6b858 + 2;
    *(p + 0xcc) = 0;
    *reinterpret_cast<u64*>(p + 0xb8) = 0;
    __builtin_memset(p + 0xbd, 0, 8);
    __builtin_memset(p + 0xd0, 0, 16);
    *(p + 0xe0) = 0;
    self->sub.vtable = g_unk_7101a59f90 + 2;
    *(p + 0xf0) = 0;
    *(p + 0xf8) = 0;
    *(p + 0x100) = 0;
    self->sub.field_0x52 = 0;
    __builtin_memset(p + 0xb0 + 0x54, 0, 8);
    __builtin_memset(p + 0xb0 + 0x59, 0, 8);
    *reinterpret_cast<u32*>(p + 0xe4) = 3;
    *(p + 0x118) = 0;
    *reinterpret_cast<u32*>(p + 0x11c) = 0;
}
