// The CTimelineComponent unit's copy of the block pool's creation (base/UnkBlockArrayPool.h), with blocks of 0x178 bytes.
#include "base/UnkBlockArrayPool.h"

void CTimelineComponent_CreateBlockArrayPool(UnkBlockArrayPool* self, u64 n, u32 flags, base::UnkAllocator* allocator) {
    UnkBlockArrayPoolCreate<0x178>(self, n, flags, allocator);
}
