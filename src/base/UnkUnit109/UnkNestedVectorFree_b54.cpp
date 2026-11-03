// Units 108 and 109 of tools/tu_map.py: three identical frees of a base::UnkVector of 0x48-byte
// records, each record starting with a vector of its own that is freed first (the records' other
// fields need no destruction). One instantiation per record type. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "types.h"

template <int Tag>
struct UnkVectorRecord48_b54a1 {
    base::UnkVector<u8> items;
    u8 field_0x28[0x20];
};

template <int Tag>
static inline void FreeVector_b54a1(base::UnkVector<UnkVectorRecord48_b54a1<Tag>>* self) {
    if (self->data) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].items.Free();
        self->count = 0;
        self->allocator->vf_0x48(self->data);
        self->capacity = 0;
    }
}

void fn_710011ef20(base::UnkVector<UnkVectorRecord48_b54a1<0>>* self) { FreeVector_b54a1(self); }
void fn_7100123fb0(base::UnkVector<UnkVectorRecord48_b54a1<1>>* self) { FreeVector_b54a1(self); }
void fn_71001254a4(base::UnkVector<UnkVectorRecord48_b54a1<2>>* self) { FreeVector_b54a1(self); }
