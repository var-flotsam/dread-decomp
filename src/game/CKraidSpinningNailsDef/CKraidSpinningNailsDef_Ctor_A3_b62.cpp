// CKraidSpinningNailsDef unit: fn_71005eaafc constructs a launch pattern on the shared base (constructor
// fn_71005dbdd4, as CAcidBlobsLaunchPattern.cpp) with two growing arrays (capacity 10, growth 15, default
// allocator) at 0xf0 and 0x118. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CCharClassKraidAttack.h"
#include "game/CKraidSpinningNailsDef.h"
#include "types.h"

struct UnkGrowArray_b62a3 {
    void* data;
    u64 field_0x8;
    u32 capacity;
    bool field_0x14;
    u32 growth;
    base::UnkAllocator* allocator;
};

struct UnkLaunchPattern_b62a3 {
    void** vtable;
    u8 unk_0x8[0xe8];
    UnkGrowArray_b62a3 field_0xf0;
    UnkGrowArray_b62a3 field_0x118;
};

void fn_71005eaafc(UnkLaunchPattern_b62a3* self) {
    fn_71005dbdd4(self);
    self->vtable = g_unk_7101a538e0 + 2;
    self->field_0xf0.data = nullptr;
    self->field_0xf0.field_0x8 = 0;
    self->field_0xf0.capacity = 10;
    self->field_0xf0.field_0x14 = true;
    self->field_0xf0.growth = 15;
    self->field_0xf0.allocator = fn_7100083990();
    self->field_0x118.data = nullptr;
    self->field_0x118.field_0x8 = 0;
    self->field_0x118.capacity = 10;
    self->field_0x118.field_0x14 = true;
    self->field_0x118.growth = 15;
    self->field_0x118.allocator = fn_7100083990();
}
