// The base::animation::CBlendSpace1D unit's copy of the block pool's creation
// (base/UnkBlockArrayPool.h), with blocks of 0x70 bytes.
#include "base/UnkBlockArrayPool.h"

void fn_7100008784(UnkBlockArrayPool* self, u64 n, u32 flags, base::UnkAllocator* allocator) {
    UnkBlockArrayPoolCreate<0x70>(self, n, flags, allocator);
}
