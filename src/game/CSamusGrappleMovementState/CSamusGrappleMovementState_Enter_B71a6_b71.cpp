// CSamusGrappleMovementState unit (tools/tu_map.py, unit 1461): entering the state with a kind. It clears
// the 2D direction at +0xba4 and its flags, then sets flags per kind; for kind 13 it takes the direction
// from the movement's state of the kind fn_71009a60fc names. Names and layouts are placeholders.
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU55.h"
#include "types.h"

struct UnkGroundPlayer_b52;
bool fn_71010d1598(UnkGroundPlayer_b52*);
struct UnkVec3;

struct UnkGrappleMoveState_B71a6 {
    void* vtable;
    u8 pad_0x8[0x8];
    s32 kind;
    u8 pad_0x14[0x1c4 - 0x14];
    f32 dir[3];
    UnkSegment2D segment;
};

#pragma pack(push, 4)
struct UnkGrappleMove_B71a6 {
    u8 pad_0x0[0x2d18];
    u32 state_count;
    UnkGrappleMoveState_B71a6* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

struct UnkGrappleState_B71a6 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    UnkGrappleMove_B71a6* movement;
    u8 pad_0x40[0xba4 - 0x40];
    f32 dir_x;
    f32 dir_y;
    bool flag_0xbac;
    bool flag_0xbad;
    bool flag_0xbae;
    bool flag_0xbaf;
    bool flag_0xbb0;
    bool flag_0xbb1;
};

struct UnkNegVec3_B71a6 {
    f32 x, y, z;
};

bool fn_71009ad430(UnkGrappleMoveState_B71a6* state);
u32 fn_71009a60fc();

static inline UnkGrappleMoveState_B71a6* FindState(UnkGrappleMove_B71a6* self, s32 kind) {
    u64 size = (u64)self->state_count * sizeof(UnkGrappleMoveState_B71a6*);
    asm("" : "+r"(size));
    if (size == 0)
        return nullptr;
    UnkGrappleMoveState_B71a6** it = self->states;
    for (; size != 0; ++it, size -= sizeof(*it)) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

void fn_710099adcc(UnkGrappleState_B71a6* self, s32 kind) {
    self->dir_x = 0;
    self->dir_y = 0;
    self->flag_0xbac = false;
    self->flag_0xbad = false;
    self->flag_0xbae = false;
    self->flag_0xbaf = false;
    self->flag_0xbb0 = false;
    self->flag_0xbb1 = false;
    switch (kind) {
    case 3:
        if (fn_710117b348(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), fn_7100c1b388()))
            break;
        if (fn_710117b348(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), fn_7100c1ae78()))
            break;
        self->flag_0xbaf = true;
        if (fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(self->movement)))
            self->flag_0xbb0 = true;
        break;
    case 4:
        self->flag_0xbb1 = true;
        break;
    case 6:
        self->flag_0xbac = true;
        break;
    case 13: {
        UnkGrappleMove_B71a6* m = self->movement;
        u32 k = fn_71009a60fc();
        UnkGrappleMoveState_B71a6* state = k <= 14 ? FindState(m, k) : nullptr;
        bool r = fn_71009ad430(state);
        self->flag_0xbad = r;
        self->flag_0xbae = !r;
        {
            UnkVec2Result n = fn_7100125e78(&state->segment);
            self->dir_x = n.x;
            self->dir_y = n.y;
        }
        if (self->dir_x + 0.001f >= 0 && self->dir_x - 0.001f <= 0 && self->dir_y + 0.001f >= 0 &&
            self->dir_y - 0.001f <= 0) {
            f32 nx = -state->dir[0], ny = -state->dir[1], nz = -state->dir[2];
            UnkNegVec3_B71a6 v;
            v.x = nx;
            v.y = ny;
            v.z = nz;
            fn_710007b444(&self->dir_x, reinterpret_cast<const UnkVec3*>(&v));
        }
        break;
    }
    default:
        break;
    }
}
