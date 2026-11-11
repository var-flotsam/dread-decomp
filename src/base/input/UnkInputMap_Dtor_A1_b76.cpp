// From unit 46 of tools/tu_map.py (the controllers' part of the input system): the destructor of
// the object holding the flat map of src/base/input/UnkInputMap_Remove_b67a1.cpp (vtable
// unk_7101a1a2b0; arrays at +0x18 (values, each a byte vector) and +0x20 (u32 ids), count at +0x28,
// allocator at +0x38): frees every value's array, then the two arrays. fn_710006dc28 and its
// identical copy fn_710006f230. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU46.h"
#include "types.h"

struct UnkInputMapA1 {
    void* vtable;
    u8 pad_0x8[0x10];
    base::UnkVector<u8>* values;  // 0x18
    u32* keys;                    // 0x20
    u32 count;                    // 0x28
    u8 pad_0x2c[0xc];
    base::UnkAllocator* allocator;  // 0x38
};

static inline void DestroyA1(UnkInputMapA1* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1a2b0) + 0x10;
    if (self->values && self->count)
        for (u64 i = 0; i < self->count; ++i)
            self->values[i].Free();
    self->count = 0;
    if (self->keys)
        self->allocator->vf_0x48(self->keys);
    if (self->values)
        self->allocator->vf_0x48(self->values);
}

void fn_710006dc28(UnkInputMapA1* self) {
    DestroyA1(self);
}

void fn_710006f230(UnkInputMapA1* self) {
    DestroyA1(self);
}
