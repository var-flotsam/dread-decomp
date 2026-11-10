// CBreakableTileGroupSonarTargetComponent's unit (tools/tu_map.py, unit 1517): (re)creating the unit's pool of
// 0x40-byte blocks, the shared template of base/UnkBlockArrayPool.h. Names are placeholders.
#include "base/UnkBlockArrayPool.h"

void fn_7100a0dd44(UnkBlockArrayPool* self, u64 n, u32 flags, base::UnkAllocator* allocator) {
    UnkBlockArrayPoolCreate<0x40>(self, n, flags, allocator);
}
