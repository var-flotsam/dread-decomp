// CDoorShieldLifeComponent unit (tools/tu_map.py, the name from reflection): the actor's position
// (+0x30), moved out along x by half the width at +0x6a4 plus 1 to the side that is open (+0x644 or
// +0x648, when only one is) unless the object at +0x9f8 has its flag at +0x128 set. Names and layouts
// are placeholders.
#include "types.h"

union UnkBits_db {
    u32 u;
    f32 f;
};

// Not trivially copyable, so it is returned in memory.
struct UnkVec3Bits_db {
    UnkBits_db x;
    UnkBits_db y;
    UnkBits_db z;

    UnkVec3Bits_db(u32 x, u32 y, u32 z) : x{x}, y{y}, z{z} {}
    UnkVec3Bits_db(const UnkVec3Bits_db& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkActorPos_db {
    u8 pad_0x0[0x44];
    u32 x;  // 0x44
    u8 pad_0x48[0xc];
    u32 y;  // 0x54
    u8 pad_0x58[0xc];
    u32 z;  // 0x64
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

UnkVec3Bits_db fn_71008db1d8(UnkDoorShieldLife_db* self) {
    UnkActorPos_db* actor = self->actor;
    UnkVec3Bits_db result(actor->x, actor->y, actor->z);
    if (self->state->field_0x128)
        return result;
    f32 x = result.x.f;
    if (self->field_0x644) {
        if (!self->field_0x648)
            result.x.f = self->width * 0.5f + x + 1.0f;
    } else if (self->field_0x648) {
        result.x.f = x + self->width * -0.5f + -1.0f;
    }
    return result;
}
