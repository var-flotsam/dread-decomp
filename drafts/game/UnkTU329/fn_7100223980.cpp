// Unit 329: the movie library's reallocate callback over the unit's two sub-allocators (blocks of
// 512 KiB and more from the big one), each step under the statics' mutex. Names and layouts are
// placeholders.
#include <nn/os.h>
#include <string.h>

#include "base/UnkDefaultAllocator.h"
#include "game/UnkTU58.h"
#include "types.h"

using base::UnkSubAllocator;

bool fn_71000814d4(UnkSubAllocator* self, void* ptr);

// The unit's statics: the "in use" flag, the mutex and two sub-allocators.
struct UnkPoolStatics_ovb0224 {
    u8 in_use;
    u8 unk_0x1[0x17];
    nn::os::MutexType mutex;  // 0x18
    base::UnkSubAllocator allocator_a;  // 0x38, "VideoNX_Big"
    base::UnkSubAllocator allocator_b;  // 0x70, "VideoNX_Small"
};
static_assert(__builtin_offsetof(UnkPoolStatics_ovb0224, allocator_b) == 0x70, "statics layout");
extern UnkPoolStatics_ovb0224 g_unk_7172699f08 __attribute__((visibility("hidden")));

// The sub-allocator a block of this size comes from.
static inline UnkSubAllocator* AllocatorFor(u64 size) {
    return size >= 0x80000 ? &g_unk_7172699f08.allocator_a : &g_unk_7172699f08.allocator_b;
}

// The sub-allocator that holds ptr, or null.
static inline void* OwnerOf(void* ptr) {
    bool in_a = fn_71000814d4(&g_unk_7172699f08.allocator_a, ptr);
    bool in_b = fn_71000814d4(&g_unk_7172699f08.allocator_b, ptr);
    return in_b ? &g_unk_7172699f08.allocator_b : in_a ? &g_unk_7172699f08.allocator_a : nullptr;
}

static inline void* Allocate(u64 size) {
    UnkSubAllocator* allocator = AllocatorFor(size);
    nn::os::LockMutex(&g_unk_7172699f08.mutex);
    void* result = fn_71000813e4(allocator, size, 8, 0x54);
    fn_7100066920(&g_unk_7172699f08.mutex);
    return result;
}

static inline void Free(void* ptr) {
    nn::os::LockMutex(&g_unk_7172699f08.mutex);
    if (void* owner = OwnerOf(ptr))
        static_cast<UnkSubAllocator*>(owner)->vf_0x48(ptr);
    fn_7100066920(&g_unk_7172699f08.mutex);
}

__attribute__((visibility("hidden"))) void* fn_7100223980(void* pointer, u64 size, void* user) {
    if (!pointer)
        return Allocate(size);
    nn::os::LockMutex(&g_unk_7172699f08.mutex);
    void* owner = OwnerOf(pointer);
    if (size == 0) {
        if (owner)
            static_cast<UnkSubAllocator*>(owner)->vf_0x48(pointer);
        fn_7100066920(&g_unk_7172699f08.mutex);
        return nullptr;
    }
    u64 old_size = owner ? static_cast<UnkSubAllocator*>(owner)->vf_0x50(pointer) : 0;
    fn_7100066920(&g_unk_7172699f08.mutex);
    if (old_size >= size)
        return pointer;
    void* result = Allocate(size);
    memcpy(result, pointer, old_size);
    Free(pointer);
    return result;
}
