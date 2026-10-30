// The base::snd::CSoundSystemATK unit's copy of the block pool's creation (base/UnkBlockArrayPool.h),
// with blocks of 0x128 bytes: the sound system's pool at +0x388, over its heap's sub-allocator
// (src/base/snd/CSoundSystemATK.cpp).
#include "base/UnkBlockArrayPool.h"
#include "base/UnkDefaultAllocator.h"

void fn_71000f86c0(void* list, u64 count, u32 flags, base::UnkSubAllocator* allocator) {
    UnkBlockArrayPoolCreate<0x128>(static_cast<UnkBlockArrayPool*>(list), count, flags,
                                   reinterpret_cast<base::UnkAllocator*>(allocator));
}
