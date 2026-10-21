// Initialises an object: runs the base initialisation, clears four words at 0x5c and sets a
// three-bit flag set to "all bits" (the complement of a shared empty set, both lazily created
// statics).
#include "types.h"

struct UnkFlags_71007142e8 {
    u32 bits;
    UnkFlags_71007142e8() : bits(0) {}
    UnkFlags_71007142e8(u32 b) : bits(b) {}
    UnkFlags_71007142e8(const UnkFlags_71007142e8& o) : bits(o.bits) {}
};

inline const UnkFlags_71007142e8& UnkFlagsNone_71007142e8() {
    static UnkFlags_71007142e8 s_none;
    return s_none;
}

inline const UnkFlags_71007142e8& UnkFlagsAll_71007142e8() {
    static UnkFlags_71007142e8 s_all(~UnkFlagsNone_71007142e8().bits & 7);
    return s_all;
}

struct UnkStruct_71007142e8 {
    u8 pad_0x0[0x5c];
    u32 field_0x5c;
    u32 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
};

void fn_7100025624(UnkStruct_71007142e8* self);
void fn_71007143c8(UnkStruct_71007142e8* self, UnkFlags_71007142e8 flags);

void fn_71007142e8(UnkStruct_71007142e8* self) {
    fn_7100025624(self);
    self->field_0x5c = 0;
    self->field_0x60 = 0;
    self->field_0x64 = 0;
    self->field_0x68 = 0;
    fn_71007143c8(self, UnkFlagsAll_71007142e8());
}
