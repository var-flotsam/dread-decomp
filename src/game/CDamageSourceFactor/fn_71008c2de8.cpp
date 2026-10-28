// Returns the multiplier for a set of damage-type flags from a table of factors: the first
// flag tested that is set selects its factor (some have a variant for flag 0x100 when one of
// the flags 0x400 / 0x8000000 is also set); 1.0 when none applies.
#include "types.h"

struct UnkStruct_71008c2de8 {
    u8 pad_0x0[0x8];
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
};

f32 fn_71008c2de8(const UnkStruct_71008c2de8* self, u32 flags) {
    if (flags & 0x10000)
        return self->field_0x1c;
    if (flags & 0x8000400) {
        bool alt = (flags & 0x100) != 0;
        if (flags & 0x1000)
            return alt ? self->field_0x3c : self->field_0x2c;
        if (flags & 0x4000)
            return alt ? self->field_0x38 : self->field_0x28;
        if (flags & 0x2000)
            return alt ? self->field_0x34 : self->field_0x24;
        if (flags & 0x1)
            return alt ? self->field_0x30 : self->field_0x20;
        return 1.0f;
    }
    if (flags & 0x1000)
        return self->field_0x14;
    if (flags & 0x4000)
        return self->field_0x10;
    if (flags & 0x2000)
        return self->field_0xc;
    if (flags & 0x1)
        return self->field_0x8;
    if (flags & 0x200000)
        return self->field_0x18;
    if (flags & 0x200)
        return self->field_0x4c;
    if (flags & 0x2)
        return self->field_0x48;
    if (flags & 0x8)
        return self->field_0x44;
    if (flags & 0x4)
        return self->field_0x40;
    if (flags & 0x20)
        return self->field_0x58;
    if (flags & 0x20000000)
        return self->field_0x54;
    if (flags & 0x10)
        return self->field_0x50;
    if (flags & 0x8000)
        return self->field_0x68;
    if (flags & 0x2000000)
        return self->field_0x64;
    if (flags & 0x40000)
        return self->field_0x60;
    if (flags & 0x40)
        return self->field_0x5c;
    return 1.0f;
}
