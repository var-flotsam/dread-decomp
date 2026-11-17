// CYamplotXAIComponent's unit (tools/tu_map.py): whether the actor (when fn_71010cc3a4 of its +0x218
// answers 1) is above its target (fn_7100d7c990) by less than 200, unless both stand at the origin.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "types.h"

using base::math::CVector3D;

struct UnkYamplotXAIComponent_l6f2c44 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

// Whether `a` lies within 0.001 of `b` on every axis.
inline bool IsNear_l6f2c44(const CVector3D& a, const CVector3D& b) {
    return a.x - 0.001f <= b.x && a.x + 0.001f >= b.x && a.y - 0.001f <= b.y && a.y + 0.001f >= b.y &&
           a.z - 0.001f <= b.z && a.z + 0.001f >= b.z;
}

bool fn_71006f2c44(UnkYamplotXAIComponent_l6f2c44* self) {
    if (fn_71010cc3a4(self->actor->field_0x218) != 1)
        return false;
    UnkActor* actor = self->actor;
    CVector3D pos{actor->field_0x44, actor->field_0x54, actor->field_0x64};
    UnkActor* target = fn_7100d7c990(actor);
    CVector3D target_pos{target->field_0x44, target->field_0x54, target->field_0x64};
    if (IsNear_l6f2c44(pos, g_unk_710164779c) && IsNear_l6f2c44(target_pos, g_unk_710164779c))
        return false;
    if (!(pos.y > target_pos.y))
        return false;
    return pos.y - target_pos.y < 200.0f;
}
