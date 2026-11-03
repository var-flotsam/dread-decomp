// Unit 222: the destructor of a keyed array of records by 8-byte key (values at +8, keys at +0x10,
// count at +0x18, allocator at +0x28; 0x98-byte records), the shape of fn_71000dc294
// (src/base/reflection/UnkReflKeyedB0_b44.cpp); the records are destroyed by fn_710019506c.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkObject_710019506c;
void fn_710019506c(UnkObject_710019506c* self);

struct UnkRecord98_b45a1 {
    u8 bytes[0x98];
};

struct UnkKeyedRecordArray_b45a1 {
    u64 field_0x0;
    UnkRecord98_b45a1* values;
    u64* keys;
    u32 count;
    u32 capacity;
    u32 field_0x18;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710019b91c(UnkKeyedRecordArray_b45a1* self) {
    if (self->values)
        for (u32 i = 0; i < self->count; ++i)
            fn_710019506c(reinterpret_cast<UnkObject_710019506c*>(&self->values[i]));
    self->count = 0;
    if (self->keys)
        self->allocator->vf_0x48(self->keys);
    if (self->values)
        self->allocator->vf_0x48(self->values);
}
