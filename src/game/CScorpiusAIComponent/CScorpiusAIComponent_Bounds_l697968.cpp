// CScorpiusAIComponent unit: the owner's bounds: the base bounds (fn_71011061d0), grown to take in points
// ahead of, behind and above the owner, further ahead when a linked object points back at the
// component, and higher while the owner plays one of the mask tracks. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkVec3 {
    f32 x, y, z;
};

struct UnkAabb {
    UnkVec3 min;
    UnkVec3 max;
};

struct UnkScorpiusLink_l697968 {
    u8 pad_0x0[0x1a0];
    void* field_0x1a0;
};

struct UnkScorpiusOwner_l697968 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x1a8];
    UnkAnimationComponent* anim;  // 0x220
    u8 pad_0x228[0x50];
    UnkScorpiusLink_l697968* field_0x278;
};
static_assert(__builtin_offsetof(UnkScorpiusOwner_l697968, anim) == 0x220, "");
static_assert(__builtin_offsetof(UnkScorpiusOwner_l697968, field_0x278) == 0x278, "");

struct UnkScorpiusAI_l697968 {
    u8 pad_0x0[0x30];
    UnkScorpiusOwner_l697968* owner;  // 0x30
    u8 pad_0x38[0x14bb8];
    u8 field_0x14bf0[8];
};
static_assert(__builtin_offsetof(UnkScorpiusAI_l697968, field_0x14bf0) == 0x14bf0, "");

static inline f32 Min(f32 a, f32 b) { return a < b ? a : b; }
static inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }

// Grows the box to take in the point.
static inline void Grow(UnkAabb* box, const UnkVec3& p) {
    box->min = UnkVec3{Min(box->min.x, p.x), Min(box->min.y, p.y), Min(box->min.z, p.z)};
    box->max = UnkVec3{Max(box->max.x, p.x), Max(box->max.y, p.y), Max(box->max.z, p.z)};
}

static inline UnkVec3 operator*(const UnkVec3& v, f32 s) { return UnkVec3{v.x * s, v.y * s, v.z * s}; }
static inline UnkVec3 operator+(const UnkVec3& a, const UnkVec3& b) { return UnkVec3{a.x + b.x, a.y + b.y, a.z + b.z}; }
static inline UnkVec3 operator-(const UnkVec3& a, const UnkVec3& b) { return UnkVec3{a.x - b.x, a.y - b.y, a.z - b.z}; }

static inline UnkVec3 Up(UnkScorpiusOwner_l697968* owner) {
    const UnkMatrix44& m = fn_710002614c(&owner->xform);
    return UnkVec3{m.m[1], m.m[5], m.m[9]};
}

static inline bool IsPlaying(UnkScorpiusAI_l697968* self, u64 track) {
    return fn_710117ae38(self->owner->anim, track, false);
}

void fn_7100697968(UnkScorpiusAI_l697968* self, UnkAabb* out) {
    fn_71011061d0(reinterpret_cast<UnkBoundsComponent*>(self), out);
    UnkScorpiusOwner_l697968* owner = self->owner;
    UnkVec3 pos = {owner->xform.m[3], owner->xform.m[7], owner->xform.m[11]};
    const UnkMatrix44& m = fn_710002614c(&owner->xform);
    UnkVec3 forward = {m.m[2], m.m[6], m.m[10]};
    Grow(out, pos + forward * 500.0f);
    Grow(out, pos - forward * 400.0f);
    Grow(out, pos + Up(self->owner) * 600.0f);

    UnkScorpiusLink_l697968* link = self->owner->field_0x278;
    if (link && link->field_0x1a0 && link->field_0x1a0 == self->field_0x14bf0)
        Grow(out, pos + forward * 1200.0f);

    UnkAnimationComponent* anim = self->owner->anim;
    if (!anim)
        return;
    if (fn_710117b348(anim, fn_7100c4b288()) || IsPlaying(self, 0x0ba06a7753ea1d73ull) ||
        IsPlaying(self, 0x4d8051d9f27d308cull) || IsPlaying(self, 0xbb71fec6bfa5dbabull) ||
        IsPlaying(self, 0x9f3eb4d219b29df9ull))
        Grow(out, pos + Up(self->owner) * 1000.0f);
}
