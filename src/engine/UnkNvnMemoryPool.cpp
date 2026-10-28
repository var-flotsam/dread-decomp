// From unit 170 of tools/tu_map.py (0x710015334c-0x7100153c20; its static initialiser is
// src/game_s1/fn_7100153ac0.cpp): the graphics start-up's NVN memory pools. Each pool's storage
// comes from the allocator g_unk_716e275190 (a dlmalloc mspace), aligned to a page, and the pool
// is built with the NVN memory-pool builder entry points (set defaults, device, flags, storage,
// then initialise); a counter keeps the number of pools made and its peak. fn_7100153350 makes
// the five fixed heaps, each also a dlmalloc mspace over its pool's storage. Names and layouts
// are placeholders.
#include <stdlib.h>

#include "base/UnkDefaultAllocator.h"
#include "types.h"

// The allocator the pools' storage comes from, and the five heaps (as
// src/game_s1/fn_7100153ac0.cpp declares them).
struct UnkPtrHolder {
    void** vtable;
    void* ptr;
};
struct UnkObject_0x130 {
    u8 field_0x0[0x100];
    u64 field_0x100;
    u64 field_0x108;
    u32 field_0x110;
    UnkPtrHolder field_0x118;
    u16 field_0x128;
};
extern UnkPtrHolder g_unk_716e275190;
extern UnkObject_0x130 g_unk_716e2751a0;
extern UnkObject_0x130 g_unk_716e2752d0;
extern UnkObject_0x130 g_unk_716e275400;
extern UnkObject_0x130 g_unk_716e275530;
extern UnkObject_0x130 g_unk_716e275660;

// NVN entry points the graphics start-up loads (data addresses only; that they are the memory-pool
// builder's and the pool's initialiser is inferred from the call sequence).
extern void (*g_unk_716e29af28)(void* builder);                            // builder: defaults
extern void (*g_unk_716e29af20)(void* builder, void* device);              // builder: device
extern void (*g_unk_716e29af38)(void* builder, s32 flags);                 // builder: flags
extern void (*g_unk_716e29af30)(void* builder, void* memory, u64 size);    // builder: storage
extern bool (*g_unk_716e29af60)(void* pool, const void* builder);          // pool: initialise

// The number of pools made and its peak (merged statics of the unit).
struct UnkPoolCount {
    u32 count;
    u32 peak;
};
extern UnkPoolCount g_unk_716e275794 __attribute__((visibility("hidden")));

// The builder, on the stack.
struct UnkNvnPoolBuilder {
    u8 data[0x40];
};

struct UnkPool;

// The graphics device (src/game_s7/fn_71001587f0.cpp).
void* fn_7100153cd0();

// A memory pool with its storage (the type fn_71001587f0 passes as UnkPool).
struct UnkNvnMemoryPool {
    u64 field_0x0;
    u8 pool[0x100];  // the NVN memory pool
    void* memory;
    u64 size;
    s32 flags;
    bool initialised;
};
static_assert(sizeof(UnkNvnMemoryPool) == 0x120, "UnkNvnMemoryPool placeholder layout");

// One of the five heaps: a memory pool, a dlmalloc mspace over its storage.
struct UnkNvnHeap {
    u8 pool[0x100];  // the NVN memory pool
    void* memory;
    u64 size;
    s32 flags;
    UnkPtrHolder allocator;  // its mspace at +8
    bool initialised;
};
static_assert(sizeof(UnkNvnHeap) == 0x130, "UnkNvnHeap placeholder layout");

// `size` rounded up to a multiple of `align` (fn_710015383c, inlined).
static inline u64 AlignUp(u64 size, u64 align) {
    return size + (align - size % align) % align;
}

// The NVN entry points are called through inline wrappers (as src/game_s7/fn_71001587f0.cpp's), so
// the arguments are evaluated before the pointer is loaded.
template <class R, class... A, class... B>
__attribute__((always_inline)) static inline R Call(R (*&fn)(A...), B... args) {
    return fn(args...);
}

// Builds the pool of `self` over its storage.
template <class T>
static inline void InitPool(T* self, void* device, s32 flags) {
    UnkNvnPoolBuilder builder;
    Call(g_unk_716e29af28, &builder);
    Call(g_unk_716e29af20, &builder, device);
    Call(g_unk_716e29af38, &builder, flags);
    Call(g_unk_716e29af30, &builder, self->memory, self->size);
    if (++g_unk_716e275794.count > g_unk_716e275794.peak)
        g_unk_716e275794.peak = g_unk_716e275794.count;
    Call(g_unk_716e29af60, self->pool, &builder);
}

void fn_710015385c(UnkPool* p, u64 size, s32 flags, void* device) {
    UnkNvnMemoryPool* self = reinterpret_cast<UnkNvnMemoryPool*>(p);
    if (size <= 0x1000)
        size = 0x1000;
    self->size = AlignUp(size, 0x1000);
    self->memory = fn_71000829f0(g_unk_716e275190.ptr, 0x1000, self->size);
    InitPool(self, device, flags);
    self->flags = flags;
    self->initialised = true;
}

// Sets up a heap: its storage, the mspace over it and its pool.
static inline void InitHeap(UnkObject_0x130* heap, u64 size, s32 flags, void* device) {
    UnkNvnHeap* self = reinterpret_cast<UnkNvnHeap*>(heap);
    if (size <= 0x1000)
        size = 0x1000;
    size = AlignUp(size, 0x1000);
    self->size = size;
    self->memory = fn_71000829f0(g_unk_716e275190.ptr, 0x100000, size);
    self->allocator.ptr = fn_71000815b0(self->memory, size, 0);
    InitPool(self, device, flags);
    self->flags = flags;
    self->initialised = true;
}

void fn_7100153350() {
    InitHeap(&g_unk_716e2752d0, 0x12c01000, 0x22, fn_7100153cd0());
    InitHeap(&g_unk_716e275530, 0x3f001000, 0x21, fn_7100153cd0());
    InitHeap(&g_unk_716e275400, 0xd001000, 0xa1, fn_7100153cd0());
    InitHeap(&g_unk_716e2751a0, 0xc01000, 0x62, fn_7100153cd0());
    InitHeap(&g_unk_716e275660, 0x101000, 0x24, fn_7100153cd0());
}

// The five heaps by index (the compiler turns the switch into a table of their addresses at
// 0x7101a1d790); any other index ends the program.
UnkObject_0x130* fn_7100153a90(s32 index) {
    switch (index) {
    case 0:
        return &g_unk_716e275660;
    case 1:
        return &g_unk_716e2752d0;
    case 2:
        return &g_unk_716e275530;
    case 3:
        return &g_unk_716e275400;
    case 4:
        return &g_unk_716e2751a0;
    }
    exit(-1);
}
