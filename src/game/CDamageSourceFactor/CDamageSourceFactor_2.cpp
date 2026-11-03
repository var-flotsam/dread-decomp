// A second copy of fn_71008c301c (CDamageSourceFactor.cpp) in the same unit: the copy constructor of
// the object with vtable 0x7101a7e198 (the word at +0x8, then the 23 floats from +0x10 as a block).
// Names and layouts are placeholders.
#include <new>

#include "types.h"

extern void* g_unk_7101a7e198[];

struct UnkDamageFactors2 {
    f32 factors[23];
};

struct UnkDamageSourceFactor2 {
    void** vtable;
    u64 field_0x8;
    UnkDamageFactors2 data;

    __attribute__((always_inline)) UnkDamageSourceFactor2(const UnkDamageSourceFactor2& other)
        : vtable(g_unk_7101a7e198 + 2), field_0x8(other.field_0x8), data(other.data) {}
};

UnkDamageSourceFactor2* fn_71008c2fc8(UnkDamageSourceFactor2* self, const UnkDamageSourceFactor2* other) {
    return new (self) UnkDamageSourceFactor2(*other);
}
