// The Slugger spit attack's update (tools/tu_map.py: the CSluggerSpitAttack unit): after the base
// update, in state 2 (+0x30) passes the owner's transform angles (owner +0x38) on with
// fn_71006c0e38; when turning is on (+0x80), moves the angle at +0xd4 toward the one at +0xd0 by at
// most pi per second, and hands it in degrees to the owner's property bag (+0x220, its +0x80).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkXformNode.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"


extern const base::global::CStrId g_unk_7172707bc0 __attribute__((visibility("hidden")));

struct UnkSluggerBagHolder_b46a2 {
    u8 pad_0x0[0x80];
    unk_prop_bag::UnkPropBag bag;
};

struct UnkSluggerOwner_b46a2 {
    u8 pad_0x0[0x38];
    UnkXform xform;
    u8 pad_0x94[0x220 - 0x94];
    UnkSluggerBagHolder_b46a2* holder;
};

struct UnkSluggerSpitAttack_b46a2 {
    u8 pad_0x0[0x8];
    UnkSluggerOwner_b46a2* owner;
    u8 pad_0x10[0x20];
    u32 state;
    u8 pad_0x34[0x80 - 0x34];
    bool turning;
    u8 pad_0x81[0xd0 - 0x81];
    f32 target_angle;
    f32 angle;
};

void fn_71006c0e38(UnkSluggerSpitAttack_b46a2* self, u32 mode, UnkVec3f* angles);

void fn_71006c25d0(UnkSluggerSpitAttack_b46a2* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    if (self->state != 2)
        return;
    UnkVec3f angles = self->owner->xform.GetAngle();
    fn_71006c0e38(self, 1, &angles);
    if (self->turning) {
        f32 step = dt * 3.14159274f;
        f32 target = self->target_angle;
        f32 angle = self->angle;
        if (target > angle) {
            f32 hi = step + angle;
            self->angle = target < hi ? target : hi;
        } else {
            f32 lo = angle - step;
            self->angle = target > lo ? target : lo;
        }
    }
    if (self->owner && self->owner->holder) {
        unk_prop_bag::fn_710033187c(&self->owner->holder->bag, g_unk_7172707bc0,
                                    (f32)((f64)(self->angle * 180.0f) / 3.141592653589793));
    }
}
