// Unit 237 (a table of indexed resources, no reliable name): the constructor of a table with three
// index arrays (by kind) and a chained map of 11 buckets. Names and layouts are placeholders.
#include "base/UnkChainedMap.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkKindA_b0253;
struct UnkKindB_b0253;
struct UnkKindC_b0253;

struct UnkResSlot_b0253 {
    void* field_0x0;   // 0x0
    void* field_0x8;   // 0x8
    void* field_0x10;  // 0x10
    void* field_0x18;  // 0x18
    UnkKindC_b0253** kind_2;  // 0x20
    UnkKindA_b0253** kind_0;  // 0x28
    UnkKindB_b0253** kind_1;  // 0x30
};
static_assert(sizeof(UnkResSlot_b0253) == 0x38, "UnkResSlot_b0253 layout");

struct UnkResTable_b0253 {
    u8 pad_0x0[0x8];
    u32 serial;  // 0x8
    u8 ready;    // 0xc
    u8 pad_0xd[0x3];
    void* owner;  // 0x10
    u8 pad_0x18[0x0];
    s32 index_2[0x181];  // 0x18
    s32 index_0[0x181];  // 0x61c
    s32 index_1[0x181];  // 0xc20
    u8 pad_0x1224[0x4];
    UnkChainedMap map;  // 0x1228
    u32 entry_count;    // 0x1270
    u32 count_2;        // 0x1274
    u32 count_0;        // 0x1278
    u32 count_1;        // 0x127c
    u32 pad_0x1280;
    s32 default_entry;  // 0x1284
    UnkResSlot_b0253* entries;  // 0x1288
    void* field_0x1290;         // 0x1290
};
static_assert(__builtin_offsetof(UnkResTable_b0253, index_0) == 0x61c, "UnkResTable_b0253 layout");
static_assert(__builtin_offsetof(UnkResTable_b0253, index_1) == 0xc20, "UnkResTable_b0253 layout");
static_assert(__builtin_offsetof(UnkResTable_b0253, map) == 0x1228, "UnkResTable_b0253 layout");
static_assert(__builtin_offsetof(UnkResTable_b0253, entries) == 0x1288, "UnkResTable_b0253 layout");

extern u32 g_unk_7101a1e570;

void fn_71001afcc4(UnkResTable_b0253* self, void* owner) {
    self->serial = g_unk_7101a1e570++;
    self->ready = 0;
    self->owner = owner;
    new (&self->map) UnkChainedMap();
    self->map.ResizeBuckets(11);
    __builtin_memset(&self->entry_count, 0, 0x28);
}
