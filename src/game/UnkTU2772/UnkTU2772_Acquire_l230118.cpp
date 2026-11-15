// nn::atk unit: takes the next free 256-byte slot from a pool and resets it.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAtkSlot {
    u8 pad_0x00[0x28];
    u8 field_0x28;
    u8 pad_0x29[0x7b];
    u32 field_0xa4;
    u8 pad_0xa8[4];
    u32 field_0xac;
    u32 field_0xb0;
    u8 pad_0xb4[4];
    u64 field_0xb8;
    u64 field_0xc0;
    u8 pad_0xc8[0x20];
    u32 field_0xe8;
    u8 pad_0xec[0x14];
};
static_assert(sizeof(UnkAtkSlot) == 0x100, "slot size");

struct UnkAtkSlotPool {
    UnkAtkSlot* slots;
    UnkAtkSlot** freeList;
    s32 nextFree;
    u8 pad_0x14[4];
    u32* slotIndex;
    s32 capacity;
};

UnkAtkSlot* fn_7101230118(UnkAtkSlotPool* pool) {
    s32 i = pool->nextFree;
    if (i >= pool->capacity)
        return nullptr;
    pool->nextFree = i + 1;
    UnkAtkSlot* slot = pool->freeList[i];
    u64 index = (u64)((u8*)slot - (u8*)pool->slots) / sizeof(UnkAtkSlot);
    pool->slotIndex[index] = i;
    slot->field_0x28 = 1;
    slot->field_0xb8 = 0;
    slot->field_0xc0 = 0;
    slot->field_0xa4 = 0;
    slot->field_0xe8 = 0;
    slot->field_0xac = 0x7d00;
    slot->field_0xb0 = 1;
    return slot;
}
