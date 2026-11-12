// Unit 170 of tools/tu_map.py, beside UnkNvnMemoryPool.cpp and UnkNvnMemoryPool_Finalize_b50.cpp (same
// globals, declared as there): fn_7100153998 tears one of the five fixed heaps down (the
// UnkObject_0x130 of UnkNvnMemoryPool.cpp): when it has storage, frees the storage from its own mspace
// if flagged (+0x129), finalises the NVN pool, counts it out, returns the storage to the allocator and
// clears it. Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "game/UnkTU170.h"
#include "game/UnkTU176.h"
#include "types.h"

struct UnkPtrHolder {
    void** vtable;
    void* ptr;
};

struct UnkPoolCount {
    u32 count;
    u32 peak;
};

// NVN entry point the graphics start-up loads (inferred: the memory pool's finaliser).

struct UnkNvnHeap_b68a1 {
    u8 pool[0x100];  // the NVN memory pool
    void* memory;    // 0x100
    u64 size;        // 0x108
    u32 field_0x110;
    UnkPtrHolder space;  // 0x118, the mspace over the storage
    bool initialised;    // 0x128
    bool owns_space;     // 0x129
};
static_assert(__builtin_offsetof(UnkNvnHeap_b68a1, owns_space) == 0x129, "layout");

template <class R, class... A, class... B>
__attribute__((always_inline)) static inline R Call_b68a1(R (*&fn)(A...), B... args) {
    return fn(args...);
}

// Returns storage to the pools' allocator.
static inline void FreeStorage_b68a1(void* memory) {
    fn_7100082370(g_unk_716e275190.ptr, memory);
}

void fn_7100153998(UnkNvnHeap_b68a1* self) {
    void* memory = self->memory;
    if (!memory)
        return;
    if (self->owns_space)
        fn_7100082370(self->space.ptr, memory);
    self->size = 0;
    Call_b68a1(g_unk_716e29af70, self->pool);
    --g_unk_716e275794.count;
    FreeStorage_b68a1(self->memory);
    self->memory = nullptr;
    self->initialised = false;
}
