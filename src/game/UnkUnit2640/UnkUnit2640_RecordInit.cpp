// Unit 2640: the initialisation of a 0x50-byte record: cleared as a whole, then an empty array
// (step 10, allocation flags 0xf, the default allocator), a scale of (1, 1) at +0x38 and the flags
// stored again after the allocator call. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"
struct UnkUnit2640Record {
    u64 field_0x0;
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x1c;
    u32 flags;
    u32 field_0x24;
    base::UnkAllocator* allocator;
    u64 field_0x30;
    f32 scale[2];
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;
};
void fn_7100ffc49c(UnkUnit2640Record* self) {
    __builtin_memset(self, 0, 0x50);
    self->step = 10;
    self->field_0x1c = true;
    self->flags = 0xf;
    self->allocator = fn_7100083990();
    self->field_0x30 = 0;
    self->scale[0] = 1.0f;
    self->scale[1] = 1.0f;
    self->field_0x40 = 0;
    self->field_0x4c = false;
}
