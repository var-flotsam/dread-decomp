// Unit 437 (tools/tu_map.py, CAimComponent by its hint): the aim angle towards the target, relative to
// `base`: from the aim action's shot pose when there is one, else from the shooting point (an offset per
// pose track) to the target, wrapped into (-pi, pi]. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

// src/game/CSamusAimAction/CSamusAimAction_Pose_ovb0115.cpp defines it with one bool fewer; this caller
// passes both (the original loads w5 and w6).
struct UnkSamusAimAction_ovb0115;
bool fn_7100c9adbc(UnkSamusAimAction_ovb0115* self, void* a, void* b, void* c, void* d, bool flag, bool flag2);

// The shooting point's offset from the owner for each pose track.
__attribute__((visibility("hidden"))) extern CVector3D g_unk_71726b1740;
__attribute__((visibility("hidden"))) extern CVector3D g_unk_71726b174c;
__attribute__((visibility("hidden"))) extern CVector3D g_unk_71726b1758;
__attribute__((visibility("hidden"))) extern CVector3D g_unk_71726b1764;
__attribute__((visibility("hidden"))) extern CVector3D g_unk_71726b1770;
__attribute__((visibility("hidden"))) extern CVector3D g_unk_71726b177c;

// The pose track names of the id table (UnkGameManagerIdTable) this function reads.
struct UnkAimPoseIds_l3493d8 {
    u8 pad_0x0[0xcb8];
    CStrId field_0xcb8;
    u8 pad_0xcc0[0xfe0 - 0xcc0];
    CStrId field_0xfe0;
    u8 pad_0xfe8[0x12e8 - 0xfe8];
    CStrId field_0x12e8;
    u8 pad_0x12f0[0x17c0 - 0x12f0];
    CStrId field_0x17c0;
    u8 pad_0x17c8[0x1d90 - 0x17c8];
    CStrId field_0x1d90;
};

static inline const UnkAimPoseIds_l3493d8& PoseIds() {
    return reinterpret_cast<const UnkAimPoseIds_l3493d8&>(GameManagerIdTable());
}

struct UnkAimOwner_l3493d8 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkAnimationComponent* animation;  // 0x220
};
static_assert(__builtin_offsetof(UnkAimOwner_l3493d8, animation) == 0x220, "layout");

struct UnkAimBlock_l3493d8 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkAimComponent_l3493d8 {
    u8 pad_0x0[0x30];
    UnkAimOwner_l3493d8* owner;  // 0x30
    u8 pad_0x38[0x680 - 0x38];
    UnkAimBlock_l3493d8* field_0x680;
};

struct UnkAimInfo_l3493d8 {
    u8 pad_0x0[0x4];
    f32 angle;  // 0x4
    u8 pad_0x8[0x18 - 0x8];
    CVector3D direction;  // 0x18
    CVector3D target;     // 0x24
    u8 pad_0x30[0x48 - 0x30];
    UnkAimBlock_l3493d8* field_0x48;
    CVector3D origin;  // 0x50
};

static inline bool Changed(f32 a, f32 b) {
    return a - 0.001f > b || a + 0.001f < b;
}

static inline f32 Yaw(UnkAimComponent_l3493d8* self) {
    return self->owner->xform.GetAngle().y;
}

void fn_71003493d8(UnkAimComponent_l3493d8* self, UnkAimInfo_l3493d8* info, f32 base) {
    if (!info->field_0x48 || !info->field_0x48->field_0x10) {
        info->angle = 0.0f;
        return;
    }
    UnkAnimationComponent* animation = self->owner->animation;
    UnkTrackAction* action = fn_710070d500(CAnimationComponent_FindTrackByNameHash(animation, 0xf4061ff25a2163d6));
    f32 angle;
    if (action && fn_710009d50c(action->vf_0x18(), fn_7100c9a598())) {
        angle = base;
        if (!fn_710117b348(animation, fn_7100c27dbc())) {
            u64 pose;
            f32 degrees = 0.0f;
            const UnkMatrix44& matrix = self->owner->xform.GetMatrix();
            pose = StringCrc64("DC_Shot", 7);
            if (fn_7100c9adbc(reinterpret_cast<UnkSamusAimAction_ovb0115*>(action), &degrees,
                              const_cast<UnkMatrix44*>(&matrix), &pose, &info->target, false, false))
                angle = degrees * 3.141592653589793 / 180.0;
        }
    } else {
        CVector3D offset;
        if (fn_710117ae38(self->owner->animation, PoseIds().field_0xfe0.data->Key(), false))
            offset = g_unk_71726b1758;
        else if (fn_710117ae38(self->owner->animation, PoseIds().field_0x1d90.data->Key(), false))
            offset = g_unk_71726b1740;
        else if (fn_710117ae38(self->owner->animation, PoseIds().field_0xcb8.data->Key(), false))
            offset = g_unk_71726b174c;
        else if (fn_710117ae38(self->owner->animation, PoseIds().field_0x12e8.data->Key(), false))
            offset = g_unk_71726b1764;
        else if (fn_710117ae38(self->owner->animation, PoseIds().field_0x17c0.data->Key(), false))
            offset = g_unk_71726b1770;
        else if (fn_710117ae38(self->owner->animation, 0x548ed9651a046f0b, false))
            offset = g_unk_71726b177c;
        else
            offset.x = offset.y = offset.z = 0.0f;

        UnkAimOwner_l3493d8* owner = self->owner;
        f32 side = Yaw(self) < 0.0f ? -offset.x : offset.x;
        CVector3D point;
        point.x = owner->xform.m[0][3] + side;
        point.y = offset.y + owner->xform.m[1][3];
        point.z = offset.z + owner->xform.m[2][3];

        const CVector3D& dir = info->direction;
        if ((Changed(dir.x, g_unk_710164779c.x) || Changed(dir.y, g_unk_710164779c.y) ||
             Changed(dir.z, g_unk_710164779c.z)) &&
            !(self->field_0x680 && self->field_0x680->field_0x10)) {
            // Onto the aim line through the origin.
            const CVector3D& o = info->origin;
            f32 t = (dir.x * (point.x - o.x) + (point.y - o.y) * dir.y + (point.z - o.z) * dir.z) /
                    (dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
            point.x = o.x + dir.x * t;
            point.y = o.y + dir.y * t;
            point.z = o.z + dir.z * t;
        }

        f32 dy = info->target.y - point.y;
        f32 dx = info->target.x - point.x;
        f32 dz = info->target.z - point.z;
        f32 inv = 1.0f / (sqrtf(dx * dx + dy * dy + dz * dz) + 0.0001f);
        dx *= inv;
        dy *= inv;
        angle = atan2f(dy, Yaw(self) < 0.0f ? -dx : dx);
        if (angle < 0.0f)
            angle += 6.2831855f;
    }
    info->angle = angle - base;
    info->angle = fn_710007a1d0(info->angle);
}
