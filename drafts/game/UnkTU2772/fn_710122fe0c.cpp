// nn::atk unit: lays out a pool of 256-byte slots in a caller-supplied buffer (free list, slot index
// table, then the 64-byte aligned slots), clears every slot and marks every index unused.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAtkWordPair {
    u32 a;
    u32 b;
};

struct UnkAtkPoolSlot {
    u64 field_0x00;
    u64 field_0x08;
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u16 field_0x28;
    u8 pad_0x2a[2];
    UnkAtkWordPair field_0x2c[15];
    u32 field_0xa4;
    u32 field_0xa8;
    u32 field_0xac;
    u32 field_0xb0;
    u8 pad_0xb4[4];
    u64 field_0xb8;
    u64 field_0xc0;
    u64 field_0xc8;
    u64 field_0xd0;
    u64 field_0xd8;
    u64 field_0xe0;
    u32 field_0xe8;
    u8 pad_0xec[0x14];
};
static_assert(sizeof(UnkAtkPoolSlot) == 0x100, "slot size");

struct UnkAtkPoolInit {
    UnkAtkPoolSlot* slots;
    UnkAtkPoolSlot** freeList;
    s32 nextFree;
    u8 pad_0x14[4];
    u32* slotIndex;
    s32 capacity;
};

void fn_710122fe0c(UnkAtkPoolInit* pool, s32 count, void* buffer) {
    pool->capacity = count;
    pool->freeList = static_cast<UnkAtkPoolSlot**>(buffer);
    pool->slotIndex = reinterpret_cast<u32*>(pool->freeList + count);
    pool->slots = reinterpret_cast<UnkAtkPoolSlot*>((reinterpret_cast<u64>(pool->slotIndex + count) + 0x3f) & ~0x3full);
    for (s32 i = 0; i < pool->capacity; i++) {
        UnkAtkPoolSlot* slot = &pool->slots[i];
        slot->field_0x28 = 0;
        slot->field_0x00 = 0;
        for (s32 k = 0; k < 15; k++) {
            slot->field_0x2c[k].a = 0;
            slot->field_0x2c[k].b = 0;
        }
        slot->field_0xa4 = 0;
        slot->field_0xa8 = 1;
        slot->field_0xac = 0x7d00;
        slot->field_0xb0 = 1;
        slot->field_0xe8 = 0;
        slot->field_0x18 = 0;
        slot->field_0x20 = 0;
        slot->field_0x08 = 0;
        slot->field_0x10 = 0;
        slot->field_0xb8 = 0;
        slot->field_0xc0 = 0;
        slot->field_0xc8 = 0;
        slot->field_0xd0 = 0;
        slot->field_0xd8 = 0;
        pool->freeList[i] = slot;
    }
    for (s32 i = 0; i < pool->capacity; i++)
        pool->slotIndex[i] = static_cast<u32>(-1);
    pool->nextFree = 0;
}
