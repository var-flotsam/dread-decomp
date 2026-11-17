// CSynchronizedBlockCameraCtrl unit: frames the player and the synchronized block: the player's box
// (widened by the margins ahead of and behind where it faces) grown to hold the block's bounds gives the
// look-at offset and the distance that fits it in the field of view. Names and layouts are placeholders.
#include <math.h>

#include "base/tunable/CTunable.h"
#include "game/CCameraBoundaryCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU311.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkCameraCtrl_ovb0049;
f32 fn_71010ee560(UnkCameraCtrl_ovb0049* self);

static inline f32 DegToRad(f32 degrees) {
    return degrees * 3.141592653589793 / 180.0;
}

static inline f32 RadToDeg(f32 radians) {
    return radians * 180.0f / 3.141592653589793;
}

struct UnkPoint_la21140 {
    f32 x, y, z;
};

struct UnkBox_la21140 {
    UnkPoint_la21140 min;
    UnkPoint_la21140 max;

    void Add(const UnkPoint_la21140& p) {
        min.x = min.x < p.x ? min.x : p.x;
        min.y = min.y < p.y ? min.y : p.y;
        min.z = min.z < p.z ? min.z : p.z;
        max.x = max.x > p.x ? max.x : p.x;
        max.y = max.y > p.y ? max.y : p.y;
        max.z = max.z > p.z ? max.z : p.z;
    }
};

struct UnkSyncPlayer_la21140 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x2f4];
    f32 width;     // 0x388
    f32 height;    // 0x38c
    u8 pad_0x390[4];
    f32 center_y;  // 0x394
};
static_assert(__builtin_offsetof(UnkSyncPlayer_la21140, center_y) == 0x394, "UnkSyncPlayer_la21140 layout");

struct UnkSyncPosition_la21140 {
    f32 x, y, z;
};

struct UnkSyncBounded_la21140 {
    void** vtable;

    void GetBounds(UnkBox_la21140* box) {
        reinterpret_cast<void (*)(UnkSyncBounded_la21140*, UnkBox_la21140*)>(vtable[0x270 / 8])(this, box);
    }
};

struct UnkSyncEntity_la21140 {
    u8 pad_0x0[0x240];
    UnkSyncBounded_la21140* bounded;  // 0x240
};

struct UnkSyncBlock_la21140 {
    u8 pad_0x0[0x10];
    UnkSyncEntity_la21140* entity;  // 0x10
};

struct UnkSyncSettings_la21140 {
    u8 pad_0x0[0xe4];
    f32 fov;  // 0xe4
};

struct UnkSyncOwner_la21140 {
    u8 pad_0x0[0x10];
    UnkSyncSettings_la21140* settings;  // 0x10
};

struct UnkSyncBlockCamera_la21140 {
    u8 pad_0x0[0x74];
    UnkVec3f field_0x74;
    u8 pad_0x80[0x14];
    f32 field_0x94;
    u8 pad_0x98[0x18];
    UnkSyncOwner_la21140* owner;  // 0xb0
    u8 pad_0xb8[0x18c];
    UnkVec3f target;  // 0x244
    u8 pad_0x250[8];
    f32 distance;  // 0x258
    u8 pad_0x25c[0x44];
    UnkSyncBlock_la21140* block;  // 0x2a0
    UnkVec3f field_0x2a8;
    u8 pad_0x2b4[0x38];
    UnkVec3f margin_back;   // 0x2ec
    UnkVec3f margin_front;  // 0x2f8
    u8 pad_0x304[4];
    f32 min_distance;    // 0x308
    f32 y_offset;        // 0x30c
    f32 y_offset_scale;  // 0x310
    u8 pad_0x314[0x10];
    UnkVec3f offset;  // 0x324

    // The block's bounded part, if it has one.
    UnkSyncBounded_la21140* Bounded() {
        if (!block || !block->entity)
            return nullptr;
        return block->entity->bounded;
    }
};
static_assert(__builtin_offsetof(UnkSyncBlockCamera_la21140, offset) == 0x324, "UnkSyncBlockCamera_la21140 layout");

void fn_7100a21140(UnkSyncBlockCamera_la21140* self) {
    fn_7100764388(reinterpret_cast<UnkPureFollowCameraCtrl*>(self));
    if (!self->owner || !self->owner->settings)
        return;
    auto* player = reinterpret_cast<UnkSyncPlayer_la21140*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;

    f32 z = reinterpret_cast<const UnkSyncPosition_la21140*>(fn_7100d7c9f4(reinterpret_cast<UnkPlayer*>(player)))->z;
    const auto* pos =
        reinterpret_cast<const UnkSyncPosition_la21140*>(fn_7100d7c9f4(reinterpret_cast<UnkPlayer*>(player)));
    f32 x = pos->x;
    f32 y = pos->y + player->center_y;
    f32 half_width = player->width * 0.5f;
    f32 half_height = player->height * 0.5f;
    UnkVec3f back = player->xform.GetAngle().y < 0.0f ? self->margin_back : self->margin_front;
    UnkVec3f front = player->xform.GetAngle().y < 0.0f ? self->margin_front : self->margin_back;

    UnkBox_la21140 frame;
    frame.min.x = (x - half_width) - back.x;
    frame.min.y = (y - half_height) - back.y;
    frame.min.z = z;
    frame.max.x = (x + half_width) + front.x;
    frame.max.y = (y + half_height) + front.y;
    frame.max.z = z;
    if (UnkSyncBounded_la21140* bounded = self->Bounded()) {
        UnkBox_la21140 bounds = {};
        bounded->GetBounds(&bounds);
        frame.Add(bounds.max);
        frame.Add(bounds.min);
    }

    f32 center_x = (frame.min.x + frame.max.x) * 0.5f;
    f32 center_y = (frame.min.y + frame.max.y) * 0.5f;
    f32 center_z = (frame.min.z + frame.max.z) * 0.5f;
    f32 extent_x = (frame.max.x - frame.min.x) * 0.5f;
    f32 half_fov = DegToRad(self->owner->settings->fov) * 0.5f;
    f32 distance_x = extent_x / tanf(half_fov);
    f32 extent_y = (frame.max.y - frame.min.y) * 0.5f;
    f32 scale = fn_7100132fa0();
    f32 half_fov_y = fn_7100217ee0(RadToDeg(half_fov), scale);
    f32 distance_y = extent_y / tanf(DegToRad(half_fov_y));
    f32 distance = distance_x > distance_y ? distance_x : distance_y;

    f32 min_distance = self->min_distance;
    self->field_0x74 = self->field_0x2a8;
    self->distance = min_distance > distance && min_distance >= 0.0f ? min_distance : distance;

    UnkVec3f translation = player->xform.GetTranslation();
    self->offset = UnkVec3f(center_x - translation.x, center_y - translation.y, center_z - translation.z);
    self->offset.y += self->y_offset;
    UnkVec3f target = player->xform.GetTranslation();
    target.y = self->y_offset + target.y;
    self->target = target;
    self->y_offset *= self->y_offset_scale;
    self->field_0x94 = fn_71010ee560(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}
