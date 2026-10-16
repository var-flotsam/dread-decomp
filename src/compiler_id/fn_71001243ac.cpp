// Two-level handle lookup: bits 16..30 select a 0x48-byte slot, the low 16 bits
// select a 12-byte entry in that slot's array; returns the entry's third word.
// Not matching: same instructions, different scheduling/register assignment.
#include "types.h"

struct UnkStruct_0xc {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

struct UnkStruct_0x48 {
    UnkStruct_0xc* entries;
    u8 unk_0x8[0x40];
};

struct UnkPool_0x48 {
    UnkStruct_0x48* slots;
    u32 count;
};

u32 fn_71001243ac(UnkPool_0x48* pool, u32 handle) {
    u32 slot = (handle >> 16) & 0x7fff;
    u32 entry = handle & 0xffff;
    return pool->slots[slot].entries[entry].field_0x8;
}
