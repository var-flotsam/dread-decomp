// CHomingMovement unit (tools/tu_map.py unit 1622): the homing target. fn_7100ac2238 keeps a counted
// weak handle (+0x5d8) to `target` (none for null) and copies the name of the point to home in on
// (+0x5e0). fn_7100ac2014 is the update: after the base update (fn_7100ac40fc), while the target is
// alive, points the direction (+0x40) from the position (+0x84) at the homing point (vf_0x370) plus
// the offset at +0x5e8, keeps the distance at +0x5d0 and normalises the direction unless it is
// zero. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CStrId.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

UnkWeakHandle* fn_7100d78378(UnkWeakTarget* self);

struct UnkHomingMovement_ovb0050 {
    u8 pad_0x0[0x5d8];
    UnkWeakHandle* target;  // 0x5d8
    CStrId point;           // 0x5e0
};

void fn_7100ac2238(UnkHomingMovement_ovb0050* self, UnkWeakTarget* target, const CStrId& point) {
    UnkWeakHandle* h = target ? fn_7100d78378(target) : nullptr;
    AssignRef(&self->target, h);
    self->point = point;
}

struct UnkHomingMovement2_ovb0050 {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 dir[3];  // 0x40
    u8 pad_0x4c[0x38];
    f32 pos[3];  // 0x84
    u8 pad_0x90[0x540];
    f32 distance;  // 0x5d0
    u8 pad_0x5d4[4];
    UnkWeakHandle* target;  // 0x5d8
    u8 pad_0x5e0[8];
    f32 offset[3];  // 0x5e8

    UnkVec3f vf_0x370() {
        return reinterpret_cast<UnkVec3f (*)(UnkHomingMovement2_ovb0050*)>(vtable[0x370 / 8])(this);
    }
};

struct UnkChargeComponent;
void fn_7100ac40fc(UnkChargeComponent*, f32);

void fn_7100ac2014(UnkHomingMovement2_ovb0050* self, f32 dt) {
    fn_7100ac40fc(reinterpret_cast<UnkChargeComponent*>(self), dt);
    if (self->target && self->target->target) {
        UnkVec3f p = self->vf_0x370();
        f32 ax = p.x + self->offset[0];
        f32 ay = p.y + self->offset[1];
        f32 az = p.z + self->offset[2];
        f32 x = ax - self->pos[0];
        f32 y = ay - self->pos[1];
        f32 z = az - self->pos[2];
        self->dir[0] = x;
        self->dir[1] = y;
        self->dir[2] = z;
        f32 len = sqrtf(x * x + y * y + z * z);
        self->distance = len;
        if (len != 0.0f) {
            self->dir[0] = self->dir[0] / len;
            self->dir[1] = self->dir[1] / len;
            self->dir[2] = self->dir[2] / len;
        }
    }
}
