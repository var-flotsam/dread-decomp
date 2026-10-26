// Sets an object's 2D scale: stores the scale matrix built from it (z scale 1), the scale
// itself twice, and two dirty flags.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

struct UnkStruct_710020a5a0 {
    u8 unk_0x0[0xec];
    CVector2D field_0xec;
    u8 unk_0xf4[0x3];
    bool field_0xf7;
    u8 unk_0xf8[0x2];
    bool field_0xfa;
    u8 unk_0xfb[0x110 - 0xfb];
    UnkMatrix44 field_0x110;
    u8 unk_0x150[0x4];
    CVector2D field_0x154;
};

void fn_710020a5a0(UnkStruct_710020a5a0* self, const CVector2D* scale) {
    UnkCopyMatrix(&self->field_0x110, fn_7100079138(scale->x, scale->y, 1.0f));
    self->field_0x154.x = scale->x;
    self->field_0x154.y = scale->y;
    self->field_0xec.x = scale->x;
    self->field_0xec.y = scale->y;
    self->field_0xf7 = true;
    self->field_0xfa = true;
}
