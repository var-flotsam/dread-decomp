// Fills a box (two corners, 0x18 bytes) around the object's position when it has a position
// object: x +-1150, y raised by 600 and then +-650, and z kept as is for both corners.
#include "game/UnkActor.h"
#include "types.h"

struct UnkPositioned {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    u32 field_0x64;
};

struct UnkBoxed {
    u8 pad_0x0[0x30];
    UnkPositioned* field_0x30;
};

struct UnkBox {
    f32 min_x;
    f32 min_y;
    u32 z;
    f32 max_x;
    f32 max_y;
    f32 max_z;
};

void fn_71004d4e18(UnkBoxed* self, UnkBox* box) {
    if (!fn_7100d7c990(reinterpret_cast<UnkActor*>(self->field_0x30)))
        return;
    UnkPositioned* p = self->field_0x30;
    f32 y = p->field_0x54 + 600.0f;
    f32 x = p->field_0x44;
    u32 z = p->field_0x64;
    f32 zf;
    __builtin_memcpy(&zf, &z, 4);
    UnkBox result;
    result.min_x = x + -1150.0f;
    result.min_y = y + -650.0f;
    result.z = z;
    result.max_x = x + 1150.0f;
    result.max_y = y + 650.0f;
    result.max_z = zf + 0.0f;
    *box = result;
}
