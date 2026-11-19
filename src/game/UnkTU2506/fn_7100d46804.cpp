// Unit 2506 (no reliable name): the constructor of the 0x6b0-byte manager the startup code creates
// (src/engine/UnkStartupManagers.cpp): its pools, its two group arrays reserved to 32 and 64 entries, a
// collision hit query and cleared words. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU2506.h"
#include "game/UnkTU365.h"
#include "types.h"

struct UnkManager_7100863a8c;
struct UnkSlotGroup_ovb0244;
struct UnkRefCountedU_ovb0244;

extern void* g_unk_7101aa8740[];
extern void* g_unk_7101aa8768[];
extern void* g_unk_7101aa8788[];
extern void* g_unk_7101aa87a8[];
extern void* g_unk_7101aa87c8[];

// A pool: a vtable, a count, two pointers, the allocator and its allocation flags.
struct UnkPool_ld46804 {
    void** vtable;
    u32 count;
    void* field_0x10;
    void* field_0x18;
    base::UnkAllocator* allocator;
    u32 flags;
};

static inline void InitPool(UnkPool_ld46804* pool, void** vtable) {
    pool->vtable = vtable;
    pool->count = 0;
    pool->field_0x10 = nullptr;
    pool->field_0x18 = nullptr;
    pool->allocator = fn_7100083990();
    pool->flags = 0x13;
}

// A pool with a word after it.
struct UnkWordPool_ld46804 {
    UnkPool_ld46804 pool;
    u32 field_0x30;
};

// The unit's out-of-line constructors of the table at +0xa0 and +0x118 (a capacity and allocation flags).
struct UnkTable_ld46804 {
    u8 pad_0x0[0x28];
};
void fn_7100d469b0(UnkTable_ld46804* self, u32 capacity, u32 flags);
void fn_7100d46aa0(UnkTable_ld46804* self, u32 capacity, u32 flags);

struct UnkTablePool_ld46804 {
    UnkPool_ld46804 pool;
    UnkTable_ld46804 table;  // +0x30
};

// The group array as src/game/UnkTU2506/UnkTU2506_Groups_ovb0244.cpp lays it out.
struct UnkGroupArray_ovb0244 {
    UnkSlotGroup_ovb0244* groups;
    UnkRefCountedU_ovb0244** handles;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};
void fn_7100d4ae50(UnkGroupArray_ovb0244* self, u32 capacity);

static inline void InitGroupArray(UnkGroupArray_ovb0244* array) {
    array->groups = nullptr;
    array->handles = nullptr;
    array->count = 0;
    array->capacity = 0;
    array->step = 10;
    array->flags = 7;
    array->allocator = fn_7100083990();
}

// What a hit query found: the hit point and the values that go with it, cleared as a whole.
struct UnkHitResult_ld46804 {
    f32 point_x, point_y, point_z;
    f32 field_0xc[6];
    u32 field_0x24[3];
};

// A collision hit query (the shape src/game/CEntity.cpp gives it), its origin cleared.
struct UnkHitQuery_ld46804 {
    void** vtable;
    void* object;
    void* field_0x10;
    s32 id;
    UnkHitResult_ld46804 hit;  // 0x1c
    u32 pad_0x4c;
    bool field_0x50;
    u32 field_0x54;
    f32 origin_x, origin_y, origin_z;
    u64 field_0x68;

    UnkHitQuery_ld46804()
        : object(nullptr), field_0x10(nullptr), id(-1), hit(), field_0x50(false), origin_x(0.0f), origin_y(0.0f),
          origin_z(0.0f), field_0x68(0) {
        vtable = g_unk_7101a20fa8 + 2;
    }
};

// Three cleared words.
struct UnkWords_ld46804 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

static inline void Clear(UnkWords_ld46804* words) {
    words->field_0x0 = 0;
    words->field_0x4 = 0;
    words->field_0x8 = 0;
}

struct UnkTagged_ld46804 {
    void** vtable;
    u32 field_0x8;
};

struct UnkTU2506Manager_ld46804 {
    UnkWordPool_ld46804 field_0x0;
    UnkWordPool_ld46804 field_0x38;
    UnkTablePool_ld46804 field_0x70;
    UnkGroupArray_ovb0244 field_0xc8;
    UnkGroupArray_ovb0244 field_0xf0;
    UnkTable_ld46804 field_0x118;
    bool field_0x140;
    UnkPool_ld46804 field_0x148;
    UnkPool_ld46804 field_0x178;
    UnkHitQuery_ld46804 query;  // +0x1a8
    UnkTagged_ld46804 field_0x218;
    u8 pad_0x228[0x47c];
    UnkWords_ld46804 field_0x6a4;
};
static_assert(__builtin_offsetof(UnkTU2506Manager_ld46804, query) == 0x1a8, "placeholder layout");
static_assert(__builtin_offsetof(UnkTU2506Manager_ld46804, field_0x6a4) == 0x6a4, "placeholder layout");
static_assert(sizeof(UnkTU2506Manager_ld46804) == 0x6b0, "placeholder layout");

void fn_7100d46804(UnkManager_7100863a8c* manager) {
    auto* self = reinterpret_cast<UnkTU2506Manager_ld46804*>(manager);
    InitPool(&self->field_0x0.pool, g_unk_7101aa8768 + 2);
    self->field_0x0.field_0x30 = 7;
    InitPool(&self->field_0x38.pool, g_unk_7101aa8788 + 2);
    self->field_0x38.field_0x30 = 7;
    InitPool(&self->field_0x70.pool, g_unk_7101aa87a8 + 2);
    fn_7100d469b0(&self->field_0x70.table, 0x10, 7);
    InitGroupArray(&self->field_0xc8);
    fn_7100d4ae50(&self->field_0xc8, 0x20);
    InitGroupArray(&self->field_0xf0);
    fn_7100d4b0f4(&self->field_0xf0, 0x40);
    fn_7100d46aa0(&self->field_0x118, 0x40, 7);
    self->field_0x140 = false;
    InitPool(&self->field_0x148, g_unk_7101aa87c8 + 2);
    InitPool(&self->field_0x178, g_unk_7101aa87c8 + 2);

    new (&self->query) UnkHitQuery_ld46804();

    self->field_0x218.vtable = g_unk_7101aa8740 + 2;
    self->field_0x218.field_0x8 = 0;
    self->field_0x218.vtable = 2 + g_unk_7101aa8740;
    Clear(&self->field_0x6a4);
}
