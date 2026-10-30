// A lookup in the CBTObserverComponent unit (unit 2216 of tools/tu_map.py): the entry a numeric id
// names in the manager's map of parallel arrays at +0x90 (base/UnkParallelFind.h; the ids are
// 32-bit words), or null.
#include "base/UnkParallelFind.h"

struct UnkObserverEntryMap {
    u8 pad_0x0[0x90];
    void** values;  // 0x90
    u32* ids;       // 0x98
    u32 count;      // 0xa0
};

void* fn_7100ca719c(void* manager, u32 id) {
    return UnkParallelFindByValue(static_cast<UnkObserverEntryMap*>(manager), id);
}
