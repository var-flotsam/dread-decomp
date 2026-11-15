// CSamusMovement's unit (unit 1501): the position of the movement's mode of kind 6 when it has one, else the
// actor's own position. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMode_l9ebe68 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
    u8 pad_0x14[0x188];
    UnkVec3f field_0x19c;
};

struct UnkActor_l9ebe68 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

// The mode pointers follow the count directly, so they sit on a 4-byte boundary.
#pragma pack(push, 4)
struct UnkMovement_l9ebe68 {
    u8 pad_0x0[0x30];
    UnkActor_l9ebe68* actor;  // 0x30
    u8 pad_0x38[0x2ce0];
    u32 mode_count;  // 0x2d18
    UnkMode_l9ebe68* modes[1];  // 0x2d1c

    UnkMode_l9ebe68* FindMode(s32 kind) {
        UnkMode_l9ebe68** end = modes + mode_count;
        for (UnkMode_l9ebe68** it = modes; it != end; ++it)
            if ((*it)->kind == kind)
                return *it;
        return nullptr;
    }
};
#pragma pack(pop)
static_assert(__builtin_offsetof(UnkMovement_l9ebe68, mode_count) == 0x2d18, "layout");
static_assert(__builtin_offsetof(UnkMovement_l9ebe68, modes) == 0x2d1c, "layout");

UnkVec3f fn_71009ebe68(UnkMovement_l9ebe68* self) {
    UnkMode_l9ebe68* mode = self->FindMode(6);
    if (mode)
        return mode->field_0x19c;
    UnkActor_l9ebe68* actor = self->actor;
    return UnkVec3f(actor->x, actor->y, actor->z);
}
