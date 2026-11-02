// In the CBTObserverComponent unit: finds the entry a numeric id names in the manager's map of
// parallel arrays at +0x90 (as fn_7100ca719c does) and, when there is one, passes it and a flag
// to fn_7100ca3518. Names and the layout are placeholders.
#include "base/UnkParallelFind.h"

struct UnkObserverEntryMap_7100ca79f0 {
    u8 pad_0x0[0x90];
    void** values;  // 0x90
    u32* ids;       // 0x98
    u32 count;      // 0xa0
};

void fn_7100ca3518(void* entry, bool flag);

void fn_7100ca79f0(UnkObserverEntryMap_7100ca79f0* manager, u32 id, bool flag) {
    void** slot = UnkParallelFindSlotByValue(manager, id);
    if (slot != &manager->values[manager->count])
        fn_7100ca3518(*slot, flag);
}
