// Unit 374 (tools/tu_map.py, no class hint): resets the 12-byte record at +0x30 (fn_710028760c), then empties the
// array of 4-byte elements at +0x8 (count +0x10, capacity +0x14): destroys each element (the empty
// fn_71002874dc), frees the storage through the allocator at +0x28 and zeroes count and capacity.
// Names and layouts are placeholders.
#include "base/UnkEmptyDtorElem.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkElem_7100286624;
void fn_710028760c(UnkElem_7100286624* self);

struct UnkElem4_A2_b74 {
    u32 value;
};

struct UnkUnit374_A2_b74 {
    u8 pad_0x0[0x8];
    UnkElem4_A2_b74* data;
    u32 count;
    u32 capacity;
    u8 pad_0x18[0x10];
    base::UnkAllocator* allocator;
    u8 record[0xc];
};

void fn_71002b52a4(UnkUnit374_A2_b74* self) {
    fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(self->record));
    if (!self->data)
        return;
    for (u64 i = 0; i < self->count; ++i)
        fn_71002874dc(&self->data[i]);
    self->count = 0;
    self->allocator->vf_0x48(self->data);
    self->capacity = 0;
}
