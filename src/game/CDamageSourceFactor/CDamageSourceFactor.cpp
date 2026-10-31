// Copy constructor of a CDamageSourceFactor-unit object (vtable 0x7101a7e198): the vtable, then
// the word at +0x8, then the 23 floats from +0x10 copied as a block. Names and layouts are placeholders.
#include <new>

#include "types.h"

extern void* g_unk_7101a7e198[];

struct UnkDamageFactors {
    f32 factors[23];
};

struct UnkDamageSourceFactor {
    void** vtable;
    u64 field_0x8;
    UnkDamageFactors data;

    __attribute__((always_inline)) UnkDamageSourceFactor(const UnkDamageSourceFactor& other)
        : vtable(g_unk_7101a7e198 + 2), field_0x8(other.field_0x8), data(other.data) {}
};

UnkDamageSourceFactor* fn_71008c301c(UnkDamageSourceFactor* self, const UnkDamageSourceFactor* other) {
    return new (self) UnkDamageSourceFactor(*other);
}
