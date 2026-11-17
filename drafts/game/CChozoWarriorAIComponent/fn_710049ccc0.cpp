// CChozoWarriorAIComponent's unit (tools/tu_map.py): steers the movement's velocity (+0x6c) during the wall
// jump: towards the jump target at the current speed while two animations play, straight at it once it
// passes the target, and scaled along x at the jump's end. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

// CChozoWarriorAIComponent's class descriptor, which its getter (fn_7100497f2c) fills and returns.
extern const base::reflection::CClass g_unk_71726d12e8 __attribute__((visibility("hidden")));

static inline const base::reflection::CClass* WarriorClass() {
    fn_7100497f2c();
    return &g_unk_71726d12e8;
}

struct UnkChozoWarriorOwner_l49ccc0 {
    u8 pad_0x0[0x38];
    f32 transform[3][4];  // 0x38
    u8 pad_0x68[0x220 - 0x68];
    void* field_0x220;
    u8 pad_0x228[0x240 - 0x228];
    base::reflection::UnkReflObject* field_0x240;
};

struct UnkChozoWarriorWalljump_l49ccc0 {
    f32 scale;
    CVector3D target;
    CVector3D dir;
};

struct UnkChozoWarriorAI_l49ccc0 {
    u8 pad_0x0[0x30];
    UnkChozoWarriorOwner_l49ccc0* owner;  // 0x30
    u8 pad_0x38[0x149ac - 0x38];
    UnkChozoWarriorWalljump_l49ccc0 walljump;  // 0x149ac
};

struct UnkChozoWarriorMovement_l49ccc0 {
    u8 pad_0x0[0x6c];
    CVector3D velocity;
};

void fn_710049ccc0(UnkChozoWarriorAI_l49ccc0* self, UnkChozoWarriorMovement_l49ccc0* move) {
    UnkChozoWarriorOwner_l49ccc0* owner = self->owner;
    base::reflection::UnkReflObject* ai = owner->field_0x240;
    if (!ai)
        return;
    auto* anim = static_cast<UnkAnimationComponent*>(owner->field_0x220);
    bool isWarrior = fn_710009d50c(ai->vf_0x0(), WarriorClass());
    if (!anim || !isWarrior)
        return;
    CVector3D vel = move->velocity;
    UnkChozoWarriorWalljump_l49ccc0* jump = &self->walljump;
    if (fn_710117ae38(anim, 0x509577d016e982c2, false) || fn_710117ae38(anim, 0xd8d3b1ea0614633a, false)) {
        const f32(*m)[4] = self->owner->transform;
        f32 dx = jump->target.x - m[0][3];
        f32 dy = jump->target.y - m[1][3];
        f32 dz = jump->target.z - m[2][3];
        f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
        dx *= inv;
        dy *= inv;
        dz *= inv;
        f32 speed = sqrtf(vel.x * vel.x + vel.y * vel.y + vel.z * vel.z);
        jump->dir.x = speed * dx;
        jump->dir.y = speed * dy;
        jump->dir.z = speed * dz;
        vel = jump->dir;
    } else if (fn_710117ae38(anim, 0x318c3cc91bcee66f, false)) {
        CVector3D dir = jump->dir;
        const f32(*m)[4] = self->owner->transform;
        f32 dot = (jump->target.x - (m[0][3] + dir.x)) * dir.x + (jump->target.y - (m[1][3] + dir.y)) * dir.y +
                  (jump->target.z - (m[2][3] + dir.z)) * dir.z;
        if (dot < 0.0f) {
            vel.x = jump->target.x - m[0][3];
            vel.y = jump->target.y - m[1][3];
            vel.z = jump->target.z - m[2][3];
        } else {
            vel = dir;
        }
    } else {
        bool ending;
        {
            CStrId end("attack_walljump_end", true);
            ending = fn_710117af10(reinterpret_cast<UnkStruct_710117af10*>(anim),
                                   reinterpret_cast<UnkCls_710117af10**>(&end), false) ||
                     fn_710117ae38(anim, 0x6dda625e7c0919e0, false);
        }
        if (!ending)
            return;
        vel.x = jump->scale * vel.x;
    }
    move->velocity = vel;
}
