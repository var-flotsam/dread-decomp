// Picks which of three states of the component the object's body is in, by how far its target
// is: the state at 0x530 when the squared distance between the object's position (values at
// 0x44, 0x54, 0x64) and the position object's is below the squared radius at 0x608, else 0x5c0 when
// it is not below the squared radius at 0x60c, else 0x578. Without an object, its body or a
// position object the answer is 0x578.
#include "types.h"

struct UnkPositioned {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
};

struct UnkObject {
    u8 pad_0x0[0x30];
    UnkPositioned* field_0x30;
};

struct UnkPicker {
    u8 pad_0x0[0x530];
    u8 field_0x530[0x48];
    u8 field_0x578[0x48];
    u8 field_0x5c0[0x48];
    f32 field_0x608;
    f32 field_0x60c;
};

UnkPositioned* fn_7100d7c990(UnkPositioned* object);

void* fn_71003f62a8(UnkPicker* self, UnkObject* object) {
    if (object) {
        UnkPositioned* body = object->field_0x30;
        if (body) {
            UnkPositioned* target = fn_7100d7c990(body);
            if (target) {
                f32 bx = body->field_0x44;
                f32 by = body->field_0x54;
                f32 bz = body->field_0x64;
                f32 tx = target->field_0x44;
                f32 ty = target->field_0x54;
                f32 tz = target->field_0x64;
                f32 dx = bx - tx;
                f32 dy = by - ty;
                f32 dz = bz - tz;
                f32 d2 = dx * dx + dy * dy + dz * dz;
                f32 near = self->field_0x608;
                if (d2 < near * near)
                    return self->field_0x530;
                f32 far = self->field_0x60c;
                if (!(d2 < far * far))
                    return self->field_0x5c0;
            }
        }
    }
    return self->field_0x578;
}
