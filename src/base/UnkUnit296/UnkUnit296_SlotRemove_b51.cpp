// From units 296 and 297 of tools/tu_map.py (0x710020f450-0x710020ffbc): pools of item pointers
// with a count, where each item keeps its own slot index (or -1 when not in the pool). Removing
// moves the last item into the freed slot. Names and layouts are placeholders.
#include "types.h"

template <u32 IndexOffset>
struct UnkSlotItem {
    u8 pad[IndexOffset];
    s32 slot;
};

template <typename Pool, typename Item>
__attribute__((always_inline)) static inline void SwapRemove(Pool* pool, Item* item) {
    s32 slot = item->slot;
    if (slot < 0)
        return;
    item->slot = -1;
    u32 last = --pool->count;
    if (static_cast<u32>(slot) != last) {
        Item* moved = pool->items[last];
        pool->items[slot] = moved;
        moved->slot = slot;
    }
}

typedef UnkSlotItem<0xdc> UnkSlotItemDc;
typedef UnkSlotItem<0xd8> UnkSlotItemD8;

struct UnkUnit296PoolA {
    u8 pad_0x0[0x6008];
    UnkSlotItemDc* items[0x800];  // 0x6008
    u8 pad_0xa008[0xc];
    u32 count;  // 0xa014
};

void fn_710020f980(UnkUnit296PoolA* pool, UnkSlotItemDc* item) {
    SwapRemove(pool, item);
}

struct UnkUnit296PoolB {
    u8 pad_0x0[0x4008];
    UnkSlotItemD8* items[0xc01];  // 0x4008
    u32 count;  // 0xa010
    u8 pad_0xa014[0x18];
    bool dirty;  // 0xa02c
};

void fn_710020fa04(UnkUnit296PoolB* pool, UnkSlotItemD8* item) {
    if (item->slot < 0)
        return;
    SwapRemove(pool, item);
    pool->dirty = true;
}

struct UnkUnit297PoolA {
    u8 pad_0x0[0x28008];
    UnkSlotItemDc* items[0x5000];  // 0x28008
    u8 pad_0x50008[0x8];
    u32 count;  // 0x50010
};

void fn_710020fba8(UnkUnit297PoolA* pool, UnkSlotItemDc* item) {
    SwapRemove(pool, item);
}

struct UnkUnit297PoolB {
    u8 pad_0x0[0x14008];
    UnkSlotItemD8* items[0x7800];  // 0x14008
    u8 pad_0x50008[0x4];
    u32 count;  // 0x5000c
};

void fn_710020fc34(UnkUnit297PoolB* pool, UnkSlotItemD8* item) {
    SwapRemove(pool, item);
}
