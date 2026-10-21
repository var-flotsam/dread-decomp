// The heap buffer's out-of-line constructor.
#include "base/UnkBuffer.h"

using base::UnkBuffer;

void fn_71000810cc(UnkBuffer* self, u64 size, base::UnkAllocator* allocator, s32 align, u32 flags) {
    new (self) UnkBuffer(size, allocator, align, flags);
}
