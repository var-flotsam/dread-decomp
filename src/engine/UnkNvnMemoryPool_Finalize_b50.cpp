// Unit 170 of tools/tu_map.py, beside UnkNvnMemoryPool.cpp (same types, declared as there): tears a
// memory pool down - finalises the NVN pool, counts it out and returns its storage to the allocator.
// Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

struct UnkPtrHolder {
    void** vtable;
    void* ptr;
};
extern UnkPtrHolder g_unk_716e275190;

struct UnkPoolCount {
    u32 count;
    u32 peak;
};
extern UnkPoolCount g_unk_716e275794 __attribute__((visibility("hidden")));

// NVN entry point the graphics start-up loads (inferred: the memory pool's finaliser).
extern void (*g_unk_716e29af70)(void* pool);

struct UnkNvnMemoryPool {
    u64 field_0x0;
    u8 pool[0x100];  // the NVN memory pool
    void* memory;
    u64 size;
    s32 flags;
    bool initialised;
};
static_assert(sizeof(UnkNvnMemoryPool) == 0x120, "UnkNvnMemoryPool placeholder layout");

// The NVN entry points are called through inline wrappers (as UnkNvnMemoryPool.cpp's), so the
// arguments are evaluated before the pointer is loaded.
template <class R, class... A, class... B>
__attribute__((always_inline)) static inline R Call(R (*&fn)(A...), B... args) {
    return fn(args...);
}

// Returns storage to the pools' allocator.
static inline void FreeStorage(void* memory) {
    fn_7100082370(g_unk_716e275190.ptr, memory);
}

void fn_71001537d4(UnkNvnMemoryPool* self) {
    if (!self->memory)
        return;
    self->size = 0;
    self->field_0x0 = 0;
    Call(g_unk_716e29af70, self->pool);
    --g_unk_716e275794.count;
    FreeStorage(self->memory);
    self->memory = nullptr;
    self->initialised = false;
}
