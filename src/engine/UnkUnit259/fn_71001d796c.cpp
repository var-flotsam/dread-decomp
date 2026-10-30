// Unit 259 of tools/tu_map.py (0x71001c6960-0x71001d948c, no reflected class)'s copy of the block pool's creation (base/UnkBlockArrayPool.h), with blocks of 0x3d0 bytes.
#include "base/UnkBlockArrayPool.h"

void fn_71001d796c(UnkBlockArrayPool* self, u64 n, u32 flags, base::UnkAllocator* allocator) {
    UnkBlockArrayPoolCreate<0x3d0>(self, n, flags, allocator);
}
