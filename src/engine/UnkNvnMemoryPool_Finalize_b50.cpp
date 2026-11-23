// Unit 170 of tools/tu_map.py, beside UnkNvnMemoryPool.cpp (same types, declared as there): tears a
// memory pool down - finalises the NVN pool, counts it out and returns its storage to the allocator.
// Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "engine/UnkNvnMemoryPool.h"
#include "engine/UnkPoolCount.h"
#include "engine/UnkPtrHolder.h"
#include "game/UnkTU170.h"
#include "game/UnkTU176.h"
#include "types.h"

// NVN entry point the graphics start-up loads (inferred: the memory pool's finaliser).

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
