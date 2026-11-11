// Unit 46 of tools/tu_map.py: the deleting destructor of the object holding the flat map of
// src/base/input/UnkInputMap_Dtor_A1_b76.cpp (vtable unk_7101a1a2b0): frees every value, the two arrays, then itself.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU46.h"
#include "types.h"

struct UnkInputMapB0232 {
    void* vtable;
    u8 pad_0x8[0x10];
    base::UnkVector<u8>* values;  // 0x18
    u32* keys;                    // 0x20
    u32 count;                    // 0x28
    u8 pad_0x2c[0xc];
    base::UnkAllocator* allocator;  // 0x38
};

void fn_710006dcfc(UnkInputMapB0232* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1a2b0) + 0x10;
    if (self->values && self->count)
        for (u64 i = 0; i < self->count; ++i)
            self->values[i].Free();
    self->count = 0;
    if (self->keys)
        self->allocator->vf_0x48(self->keys);
    if (self->values)
        self->allocator->vf_0x48(self->values);
    __builtin_operator_delete(self);
}
