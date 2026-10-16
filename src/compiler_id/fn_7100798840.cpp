// Returns field_0x2a8 unless field_0x2c has reached 0.85.
#include "types.h"

struct UnkStruct_0x2b0 {
    u8 unk_0x0[0x2c];
    f32 field_0x2c;
    u8 unk_0x30[0x2a8 - 0x30];
    f32 field_0x2a8;
};

f32 fn_7100798840(const UnkStruct_0x2b0* self) {
    if (self->field_0x2c >= 0.85f)
        return 0.0f;
    return self->field_0x2a8;
}
