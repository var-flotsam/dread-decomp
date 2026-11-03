#include "types.h"

struct UnkUnit227Values {
    u8 unk_0x0[0x3c];
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    u8 unk_0x48[0x4c];
    f32 field_0x94;
};

bool fn_710019fd3c(const UnkUnit227Values* self) {
    if (!(self->field_0x94 > 0.0f))
        return false;
    return self->field_0x3c > 0.0f || self->field_0x40 > 0.0f || self->field_0x44 > 0.0f;
}
