// Copies of fn_7100124344 (src/compiler_id/fn_7100124344.cpp), which the binary
// repeats at other addresses: fn_710012431c.
// Resolve a 32-bit handle to a 0x48-byte pool slot; the slot index lives in
// bits 16..30 of the handle.
#include "types.h"

struct UnkStruct_0x48 {
    u8 unk_0x0[0x48];
};

struct UnkPool_0x48 {
    UnkStruct_0x48* slots;
    u32 count;
};

UnkStruct_0x48* fn_710012431c(UnkPool_0x48* pool, u32 handle) {
    u32 index = (handle >> 16) & 0x7fff;
    if (index < pool->count)
        return &pool->slots[index];
    return nullptr;
}

