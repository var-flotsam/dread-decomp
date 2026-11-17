// In the CSamusMagnetGloveMovementState::CParams unit: follows the owner's input direction. A track stays active
// while the input is strong enough and its direction stays within an angle of the tracked one; the last input is
// kept, and a pending turn (+0x194) is dropped once the input points low enough. Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU437.h"
#include "types.h"

struct UnkMagnetInputVec_l9ad610 {
    f32 x, y, z;
};

struct UnkMagnetInput_l9ad610 {
    u8 pad_0x0[0x9ec];
    UnkMagnetInputVec_l9ad610 dir;  // 0x9ec
};

struct UnkMagnetOwner_l9ad610 {
    u8 pad_0x0[0x1d8];
    UnkMagnetInput_l9ad610* input;  // 0x1d8
};

struct UnkMagnetState_l9ad610 {
    u8 pad_0x0[0x30];
    UnkMagnetOwner_l9ad610* owner;  // 0x30
    u8 pad_0x38[0x15c];
    f32 field_0x194;
};
static_assert(__builtin_offsetof(UnkMagnetState_l9ad610, field_0x194) == 0x194, "layout");

struct UnkMagnetVec2_l9ad610 {
    f32 x, y;
};

struct UnkMagnetTrack_l9ad610 {
    bool active;
    UnkMagnetVec2_l9ad610 dir;   // 0x4
    UnkMagnetVec2_l9ad610 last;  // 0xc
    f32 strength;                // 0x14
};

struct UnkMagnetTunables_l9ad610 {
    u8 pad_0x0[0x48];
    f32 max_angle;     // 0x48
    f32 min_strength;  // 0x4c
};
extern UnkMagnetTunables_l9ad610 g_unk_717273c650;

struct UnkAimLimits_l9ad610 {
    u8 pad_0x0[0xa4];
    f32 low_angle;  // 0xa4
};

static inline UnkMagnetVec2_l9ad610 Normalized(const UnkMagnetVec2_l9ad610& v) {
    f32 len = sqrtf(v.x * v.x + v.y * v.y);
    if (len != 0.0f) {
        f32 inv = 1.0f / len;
        return {inv * v.x, inv * v.y};
    }
    return v;
}

void fn_71009ad610(UnkMagnetState_l9ad610* self, UnkMagnetTrack_l9ad610* track) {
    UnkMagnetInputVec_l9ad610 input = self->owner->input->dir;
    UnkMagnetVec2_l9ad610 cur = {input.x, input.y};
    track->strength = input.z;
    if (g_unk_717273c650.min_strength > input.z)
        track->active = false;
    if (track->active) {
        UnkMagnetVec2_l9ad610 a = Normalized(cur);
        UnkMagnetVec2_l9ad610 b = Normalized(track->dir);
        f32 dot = a.x * b.x + a.y * b.y;
        dot = !(dot >= 1.0f) ? dot : 1.0f;
        dot = !(dot < -1.0f) ? dot : -1.0f;
        if (g_unk_717273c650.max_angle < acosf(dot))
            track->active = false;
    }
    track->last = cur;
    if (self->field_0x194 > 0.0f) {
        const UnkAimLimits_l9ad610& limits = reinterpret_cast<const UnkAimLimits_l9ad610&>(g_unk_71726b1438);
        if (atan2f(cur.y, fabsf(cur.x)) < limits.low_angle)
            self->field_0x194 = 0.0f;
    }
}
