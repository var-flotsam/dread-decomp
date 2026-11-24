#pragma once

// Unit 180's three uniform tables: each holds its ranges (by key, and as a plain array) and the
// blocks they live in, one buffer per frame. Names and layouts are placeholders.
#include "base/UnkCloneCalls.h"
#include "types.h"

struct UnkUniformBlock {
    u8 pad_0x00[0x38];
    u64 field_0x38;     // size in bytes
    void* buffers[10];  // one per frame
    u8 pad_0x90[0x40];
    u8 field_0xd0;      // changed
    u8 pad_0xd1[7];
};
static_assert(sizeof(UnkUniformBlock) == 0xd8);

// A range as fn_710015e2d0 reads it.
struct UnkUniformRange {
    u8 pad_0x00[0x30];
    u32 offset;
    u32 size;
    u32 block;
    u8 pad_0x3c[4];
};
static_assert(sizeof(UnkUniformRange) == 0x40);

// A range as the key lookup fn_710015df40 reads it.
struct UnkUniformRangeEntry {
    u8 pad_0x00[0x38];
    u32 block;
    u32 index;
};
static_assert(sizeof(UnkUniformRangeEntry) == 0x40);

struct UnkUniformRangeTag;

struct UnkUniformTable {
    u8 pad_0x00[0x4c];
    u32 block_count;  // 0x4c
    u8 pad_0x50[0x28];
    base::UnkKeyedArray<UnkUniformRangeTag, u64, UnkUniformRangeEntry> ranges_by_key;  // 0x78
    u8 pad_0x90[0x18];
    UnkUniformRange* ranges;  // 0xa8
    UnkUniformBlock* blocks;  // 0xb0
    u8 field_0xb8;            // changed
};
static_assert(__builtin_offsetof(UnkUniformTable, ranges) == 0xa8);
static_assert(__builtin_offsetof(UnkUniformTable, blocks) == 0xb0);

struct UnkUniformTables {
    UnkUniformTable* tables[3];
};
