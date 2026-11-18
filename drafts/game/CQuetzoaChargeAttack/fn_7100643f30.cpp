// CQuetzoaChargeAttack's unit (tools/tu_map.py): aims the charge. After the base step (fn_71003608ac) it
// keeps the owner's position, and sends the owner's navigation (+0x298) a kind-5 parameter block
// (game_s2/UnkParams.h) facing the owner's forward axis, or the direction to the target when that lies
// between 70 degrees below and 20 degrees above the facing side. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkQuetzoaChargeVec_l643f30 {
    f32 x, y, z;
};

// The facing a parameter block carries (its +0x8), and whether it is set.
struct UnkQuetzoaChargeFacing_l643f30 {
    UnkVec3f dir;
    bool flag;
};

static inline UnkQuetzoaChargeVec_l643f30 Position(const UnkActor* actor) {
    UnkQuetzoaChargeVec_l643f30 pos = {actor->field_0x44, actor->field_0x54, actor->field_0x64};
    return pos;
}

struct UnkQuetzoaChargeAttack_l643f30 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x20];
    UnkQuetzoaChargeVec_l643f30 origin;  // 0x30
};

void fn_7100643f30(UnkQuetzoaChargeAttack_l643f30* self) {
    fn_71003608ac(reinterpret_cast<UnkAttackBase*>(self));
    UnkActor* owner = self->owner;
    void* nav = owner->field_0x298;
    if (!nav)
        return;
    self->origin = Position(owner);
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    UnkVec3f forward(m.m[2], m.m[6], m.m[10]);
    UnkVec3f dir = forward;
    if (UnkActor* target = fn_7100d7c990(self->owner)) {
        f32 side = (f32)((dir.x > 0.0f) - (dir.x < 0.0f));
        f32 dx = target->field_0x44 - self->origin.x;
        f32 dy = target->field_0x54 - self->origin.y;
        f32 angle = atan2f(dy, dx * side);
        if (angle >= -1.2217305f && angle <= 0.34906584f) {
            dir = UnkVec3f(cosf(angle) * side, sinf(angle), 0.0f);
        }
    }
    UnkParams params;
    params.field_0x1c = 0;
    __builtin_memset(params.field_0x8, 0, 13);
    params.field_0x30 = 0;
    __builtin_memset(params.field_0x20, 0, 16);
    params.vtable = g_unk_7101a25b10 + 2;
    params.field_0x39 = 0;
    params.field_0x3b = 0;
    params.field_0x3d = 0;
    params.field_0x34 = 5;
    params.field_0x18 = 0;
    params.field_0x1c = 1;
    UnkQuetzoaChargeFacing_l643f30* facing = reinterpret_cast<UnkQuetzoaChargeFacing_l643f30*>(params.field_0x8);
    facing->dir = dir;
    facing->flag = true;
    fn_71006f4b0c(nav, &params, 1);
}
