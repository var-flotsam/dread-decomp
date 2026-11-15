// CBTObserverComponent unit: finds the entry a numeric id names in the manager's map of parallel
// arrays at +0x128 and, when there is one, hands it and the id to fn_7100ca0118. Names and layouts are placeholders.
#include "base/UnkParallelFind.h"

struct UnkObserverSlot {
    u8 pad_0x0[0x80];
};

struct UnkObserverSlotMap {
    u8 pad_0x0[0x128];
    UnkObserverSlot* values;  // 0x128
    u32* ids;                 // 0x130
    u32 count;                // 0x138
};

struct UnkTileEventSet;
void fn_7100ca0118(UnkTileEventSet* set, const void* event);

bool fn_7100caacd0(UnkObserverSlotMap* manager, const u32* id) {
    UnkObserverSlot* slot = UnkParallelFindSlotByValue(manager, *id);
    if (slot != &manager->values[manager->count])
        fn_7100ca0118(reinterpret_cast<UnkTileEventSet*>(slot), id);
    return false;
}
