// CEnvironmentData transition unit (tools/tu_map.py): constructor of a record with unit scales. Names and layouts
// are placeholders.
#include "types.h"

struct UnkTransitionInit_A6 {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    f32 field_0x28;
    bool field_0x2c;
};

void fn_7100f739c8(UnkTransitionInit_A6* self) {
    __builtin_memset(self, 0, sizeof(*self));
    self->field_0x0 = 1.0f;
    self->field_0x4 = 0.0f;
    self->field_0x8 = 0.0f;
    self->field_0xc = 0.0f;
    self->field_0x10 = 0.0f;
    self->field_0x14 = 0.0f;
    self->field_0x18 = 1.0f;
    self->field_0x1c = 1.0f;
    self->field_0x20 = 1.0f;
    self->field_0x24 = 1.0f;
    self->field_0x28 = 0.0f;
    self->field_0x2c = false;
}
