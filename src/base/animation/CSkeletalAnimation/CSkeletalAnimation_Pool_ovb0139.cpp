// The base::animation::CSkeletalAnimation unit's copy of the block pool's creation (base/UnkBlockArrayPool.h), with blocks of 0x20 bytes.
#include "base/UnkBlockArrayPool.h"

void fn_7100013f6c(UnkBlockArrayPool* self, u64 n, u32 flags, base::UnkAllocator* allocator) {
    UnkBlockArrayPoolCreate<0x20>(self, n, flags, allocator);
}
