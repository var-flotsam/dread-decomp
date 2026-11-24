// In the CSamusMagnetGloveMovementState::CParams unit: fn_71009aea30 handles the release of the magnet surface.
// When the input points away from the surface it requests the track to run (on a wall or floor) or the
// 45-degree ceiling track up or down, else the movement's default track, and plays it. Names and layouts
// are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAnimationComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU437.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

struct UnkDesc_SpitSend;
extern UnkDesc_SpitSend g_unk_710164b958;
extern UnkTunable g_unk_717273c650;

struct UnkTrackTime_ovb0021 {
    u64 key;
    u64 time;
};
struct UnkTrackBlend_ovb0021 {
    u32 a, b, c;
};

// What a track request is built from (0x30 bytes): no id, the default time and blend descriptors,
// no flags and an all-bits mask.
struct UnkTrackParams_ovb0021 {
    s64 id;
    UnkTrackTime_ovb0021 time;
    UnkTrackBlend_ovb0021 blend;
    u32 flags;
    u32 mask;

    __attribute__((always_inline)) UnkTrackParams_ovb0021(UnkTrackTime_ovb0021 time_, UnkTrackBlend_ovb0021 blend_, u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    __attribute__((always_inline)) UnkTrackParams_ovb0021()
        : UnkTrackParams_ovb0021(reinterpret_cast<const UnkTrackTime_ovb0021&>(g_unk_710164b958),
                                 reinterpret_cast<const UnkTrackBlend_ovb0021&>(g_unk_7172769740),
                                 base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

struct UnkTrackMovement_ovb0021;
bool CCharacterMovement_BuildRequestParamsFromRecordOfClass(UnkTrackMovement_ovb0021* self, const base::reflection::CClass* cls, UnkTrackParams_ovb0021* params,
                   bool forced);
UnkTrackParams_ovb0021 fn_71009767f4(UnkTrackMovement_ovb0021* self, bool forced);

struct UnkMagnetTunable_l9aea30 {
    u8 pad_0x0[0x48];
    f32 min_release_dot;  // 0x48
};

struct UnkAimLimits_l9aea30 {
    u8 pad_0x0[0xa4];
    f32 max_angle;  // 0xa4
    u8 pad_0xa8[0x8];
    f32 min_angle;  // 0xb0
};

struct UnkMagnetOwner_l9aea30 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkMagnetState_l9aea30 {
    u8 pad_0x0[0x30];
    UnkMagnetOwner_l9aea30* owner;        // 0x30
    UnkTrackMovement_ovb0021* movement;  // 0x38
    u8 pad_0x40[0x180];
    u32 surface;  // 0x1c0
    u8 pad_0x1c4[0xc];
    UnkSegment2D segment;  // 0x1d0
    u8 pad_0x1e8[0x6c];
    bool use_direction;  // 0x254
    u8 pad_0x255[0xb];
    CVector2D input;  // 0x260
    f32 input_length;  // 0x268
    CVector3D direction;  // 0x26c
};
static_assert(__builtin_offsetof(UnkMagnetState_l9aea30, direction) == 0x26c, "layout");

struct UnkMessageTarget_l9aea30 {
    u8 pad_0x0[0x14];
    s32 field_0x14;
};

struct UnkMessageInfo_l9aea30 {
    u8 pad_0x0[0x10];
    UnkMessageTarget_l9aea30* target;  // 0x10
};

struct UnkMessage_l9aea30 {
    s32 kind;
    UnkMessageInfo_l9aea30* info;  // 0x8
};

inline bool IsNear(f32 value, f32 target, f32 tolerance) {
    return value - tolerance <= target && value + tolerance >= target;
}

// Lets go of the default action track before another track is requested.
inline void ClearDefaultAction(UnkAnimationComponent* animation) {
    UnkTrackQuery query = {fn_7100cfabc8(), nullptr, -1.0f};
    fn_710117b08c(animation, &query);
}

template <const base::reflection::CClass* (*Track)()>
inline void RequestTrack(UnkAnimationComponent* animation, UnkTrackMovement_ovb0021* movement,
                         UnkTrackParams_ovb0021* params) {
    ClearDefaultAction(animation);
    CCharacterMovement_BuildRequestParamsFromRecordOfClass(movement, Track(), params, true);
}

// The track to let go of the surface with when the input points away from it enough, else none.
static UnkTrackParams_ovb0021 ReleaseTrack(UnkMagnetState_l9aea30* self) {
    UnkTrackParams_ovb0021 transition;
    if (IsNear(self->input_length, 0.0f, 0.001f))
        return transition;
    bool release = false;
    if (self->use_direction) {
        const CVector3D& d = self->direction;
        f32 length = sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
        f32 inv = 1.0f / (length + 0.0001f);
        CVector3D unit = {self->direction.x * inv, self->direction.y * inv, self->direction.z * inv};
        CVector2D flat;
        fn_710007b430(&flat, &unit);
        const UnkMagnetTunable_l9aea30& tunable =
            reinterpret_cast<const UnkMagnetTunable_l9aea30&>(g_unk_717273c650);
        release = flat.x * self->input.x + flat.y * self->input.y >= tunable.min_release_dot;
    } else if (!(IsNear(self->direction.x, 0.0f, 0.001f) && IsNear(self->direction.y, 0.0f, 0.001f) &&
                 IsNear(self->direction.z, 0.0f, 0.001f))) {
        f32 angle = atan2f(self->input.y, fabsf(self->input.x));
        const UnkAimLimits_l9aea30& limits = reinterpret_cast<const UnkAimLimits_l9aea30&>(g_unk_71726b1438);
        switch (self->surface) {
        case 0:
            release = angle >= limits.max_angle || angle <= limits.min_angle;
            break;
        case 1:
            release = angle < limits.max_angle && angle > limits.min_angle;
            break;
        case 2: {
            UnkVec2Result normal = fn_7100125e78(&self->segment);
            f32 dot = normal.x * self->input.x + normal.y * self->input.y;
            f32 normal_length = sqrtf(normal.x * normal.x + normal.y * normal.y);
            f32 input_length = sqrtf(self->input.x * self->input.x + self->input.y * self->input.y);
            release = acosf(dot / (normal_length * input_length)) < 2.7488935f;
            break;
        }
        }
    }
    if (release) {
        if (self->surface < 2) {
            RequestTrack<fn_7100d29268>(self->owner->animation, self->movement, &transition);
        } else if (self->surface == 2) {
            const UnkSegment2D& seg = self->segment;
            f32 dx = seg.end.x - seg.start.x;
            f32 dy = seg.end.y - seg.start.y;
            f32 length = sqrtf(dx * dx + dy * dy);
            if (length != 0.0f) {
                f32 inv = 1.0f / length;
                dx *= inv;
                dy *= inv;
            }
            f32 along = dx * self->input.x + dy * self->input.y;
            UnkVec2Result normal = fn_7100125e78(&seg);
            f32 side = normal.x * 1.0f + normal.y * 0.0f;
            if ((along < 0.0f && side > 0.0f) || (along > 0.0f && !(side > 0.0f)))
                RequestTrack<fn_7100c3b9dc>(self->owner->animation, self->movement, &transition);
            else
                RequestTrack<fn_7100c3b47c>(self->owner->animation, self->movement, &transition);
        }
    }
    return transition;
}

void fn_71009aea30(UnkMagnetState_l9aea30* self, const UnkMessage_l9aea30* msg) {
    if (msg->kind != 1 || msg->info->target->field_0x14 != 0)
        return;
    UnkTrackParams_ovb0021 transition = ReleaseTrack(self);
    if (transition.id == -1) {
        UnkTrackQuery query = {fn_7100cfabc8(), nullptr, -1.0f};
        fn_710117b08c(self->owner->animation, &query);
        transition = fn_71009767f4(self->movement, true);
        if (transition.id == -1)
            return;
    }
    CAnimationComponent_SendActionRequestRecordToTrack(reinterpret_cast<UnkStruct_7101176ea8*>(self->owner->animation),
                  reinterpret_cast<const UnkRequest_7101176ea8*>(&transition), 0xf4061ff25a2163d6);
}
