// CDoorShieldLifeComponent unit (tools/tu_map.py, the name from reflection): the actor's position
// (+0x30), moved out along x by half the width at +0x6a4 plus 1 to the side that is open (+0x644 or
// +0x648, when only one is) unless the object at +0x9f8 has its flag at +0x128 set. Names and layouts
// are placeholders.
#include "game/UnkVec3Bits.h"
#include "types.h"

struct UnkActorPos_db {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkDoorShieldState_db {
    u8 pad_0x0[0x128];
    bool field_0x128;
};

struct UnkDoorShieldLife_db {
    u8 pad_0x0[0x30];
    UnkActorPos_db* actor;
    u8 pad_0x38[0x60c];
    u32 field_0x644;
    u32 field_0x648;
    u8 pad_0x64c[0x58];
    f32 width;  // 0x6a4
    u8 pad_0x6a8[0x350];
    UnkDoorShieldState_db* state;  // 0x9f8
};

UnkVec3Bits fn_71008db1d8(UnkDoorShieldLife_db* self) {
    UnkActorPos_db* actor = self->actor;
    UnkVec3Bits result = UnkVec3Bits::CopyBits(actor->x, actor->y, actor->z);
    if (self->state->field_0x128)
        return result;
    f32 x = result.x;
    if (self->field_0x644) {
        if (!self->field_0x648)
            result.x = self->width * 0.5f + x + 1.0f;
    } else if (self->field_0x648) {
        result.x = x + self->width * -0.5f + -1.0f;
    }
    return result;
}
