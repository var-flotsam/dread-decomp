// Constructors of two launch patterns (CAcidBlobsLaunchPattern, fn_71005d69b8, and
// CBouncingCreaturesLaunchPattern, fn_71005dfcc4) that share a base (constructor fn_71005dbdd4):
// set the vtable, make the string id at 0xf0 the empty string (creating the pool first when there
// is none) and set up the array after it (capacity 10, growth 15, default allocator).
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

void fn_71005dbdd4(void* self);

struct UnkGrowArray_71005d69b8 {
    void* data;
    u64 field_0x8;
    u32 capacity;
    bool field_0x14;
    u32 growth;
    base::UnkAllocator* allocator;
};

struct UnkLaunchPattern_71005d69b8 {
    void** vtable;
    u8 unk_0x8[0xe8];
    UnkStringData* field_0xf0;
    u8 unk_0xf8[0x18];
    UnkGrowArray_71005d69b8 field_0x110;
};

extern void* g_unk_7101a53428[];

void fn_71005d69b8(UnkLaunchPattern_71005d69b8* self) {
    fn_71005dbdd4(self);
    self->vtable = g_unk_7101a53428 + 2;
    self->field_0xf0 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0xf0 = &g_unk_7101d47108->empty;
    self->field_0x110.capacity = 10;
    self->field_0x110.data = nullptr;
    self->field_0x110.field_0x8 = 0;
    self->field_0x110.field_0x14 = true;
    self->field_0x110.growth = 15;
    self->field_0x110.allocator = fn_7100083990();
}

// The same for CBouncingCreaturesLaunchPattern, whose array follows the string id directly.
struct UnkLaunchPattern_71005dfcc4 {
    void** vtable;
    u8 unk_0x8[0xe8];
    UnkStringData* field_0xf0;
    UnkGrowArray_71005d69b8 field_0xf8;
};

extern void* g_unk_7101a53618[];

void fn_71005dfcc4(UnkLaunchPattern_71005dfcc4* self) {
    fn_71005dbdd4(self);
    self->vtable = g_unk_7101a53618 + 2;
    self->field_0xf0 = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->field_0xf8.data = nullptr;
    self->field_0xf0 = &g_unk_7101d47108->empty;
    self->field_0xf8.capacity = 10;
    self->field_0xf8.field_0x14 = true;
    self->field_0xf8.field_0x8 = 0;
    self->field_0xf8.growth = 15;
    self->field_0xf8.allocator = fn_7100083990();
}
