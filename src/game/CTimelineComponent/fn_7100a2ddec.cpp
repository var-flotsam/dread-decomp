// The CTimelineComponent unit's copy of the block pool's creation (base/UnkBlockArrayPool.h), with blocks of 0x178 bytes.
#include "base/UnkBlockArrayPool.h"

void fn_7100a2ddec(UnkBlockArrayPool* self, u64 n, u32 flags, base::UnkAllocator* allocator) {
    UnkBlockArrayPoolCreate<0x178>(self, n, flags, allocator);
}
