// CSamusGroundMovementState unit (unit 1462 of tools/tu_map.py): turns the actor (+0x30) round,
// fn_710099d76c. Unless the player (+0x38) is in state 3 (+0x2d08, +0x10), has nothing at +0x128
// (+0x10), or fn_71010d1598 or fn_71009e1fd4 says no, and only when the actor's facing (the third
// column of its matrix) points against the player's direction (+0x154), the opposite facing is
// turned into rotation angles (fn_710007a7f0) and set through the actor's vtable slot 0xd8.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkGroundPlayerState_b52 {
    u8 unk_0x0[0x10];
    s32 id;
};

struct UnkGroundPlayerSlot_b52 {
    u8 unk_0x0[0x10];
    void* field_0x10;
};

struct UnkGroundPlayer_b52 {
    u8 unk_0x0[0x128];
    UnkGroundPlayerSlot_b52* field_0x128;
    u8 unk_0x130[0x24];
    f32 dir_x;  // 0x154
    f32 dir_y;
    f32 dir_z;
    u8 unk_0x160[0x2d08 - 0x160];
    UnkGroundPlayerState_b52* state;  // 0x2d08
};

bool fn_71010d1598(UnkGroundPlayer_b52* player);
bool fn_71009e1fd4(UnkGroundPlayer_b52* player);

typedef void (*UnkGroundSetAngles_b52)(UnkActor*, const UnkVec3f*, bool);

struct UnkGroundMovementState_b52 {
    u8 unk_0x0[0x30];
    UnkActor* actor;
    UnkGroundPlayer_b52* player;
};

void fn_710099d76c(UnkGroundMovementState_b52* self) {
    UnkGroundPlayer_b52* player = self->player;
    if (player->state && player->state->id == 3)
        return;
    if (!player->field_0x128 || !player->field_0x128->field_0x10)
        return;
    if (!fn_71010d1598(player) || !fn_71009e1fd4(self->player))
        return;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38));
    f32 x = m.m[2], y = m.m[6], z = m.m[10];
    UnkGroundPlayer_b52* p = self->player;
    if (!(x * p->dir_x + y * p->dir_y + z * p->dir_z < 0.0f))
        return;
    UnkActor* actor = self->actor;
    const UnkMatrix44& n = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
    UnkVec3f angles = ((UnkVec3f(*)(const UnkVec3f&))fn_710007a7f0)(UnkVec3f(-n.m[2], -n.m[6], -n.m[10]));
    reinterpret_cast<UnkGroundSetAngles_b52>((*reinterpret_cast<void***>(actor))[0xd8 / 8])(actor, &angles, true);
}
