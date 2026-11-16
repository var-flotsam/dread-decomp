// Unit 1428 (CMorphBallMovement neighbourhood): fn_7100949b7c picks the direction the morph ball is pushed
// along: a segment's stored direction when the owner touches it, else away from a segment end or a point, else
// the reversed stored normal (up when nothing applies). Names and layouts are placeholders.

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkActor.h"
#include "game/UnkVec3fOps.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool fn_7100126018(const UnkSegment2D* seg, const CVector2D* point, CVector2D* out, f32 tolerance);
extern const f32 g_unk_7101647774;

struct UnkMorphBallSegment_l949b7c {
    CVector2D start;
    CVector2D end;
    f32 field_0x10;

    bool Touches(const CVector2D* point, f32 tolerance) const {
        return fn_7100126018(reinterpret_cast<const UnkSegment2D*>(this), point, nullptr, tolerance);
    }
};

struct UnkMorphBallSurface_l949b7c {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkMorphBallMovement_l949b7c {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0xc1];
    bool field_0xf9;
    u8 pad_0xfa[0x2e];
    UnkMorphBallSurface_l949b7c* field_0x128;
    u8 pad_0x130[0xc];
    f32 field_0x13c;
    f32 field_0x140;
    u8 pad_0x144[0x4];
    CVector3D field_0x148;
    u8 pad_0x154[0x22dc];
    void* field_0x2430;
    u8 pad_0x2438[0x8];
    UnkMorphBallSegment_l949b7c field_0x2440;
    UnkMorphBallSegment_l949b7c field_0x2454;
    UnkVec3f field_0x2468;
    UnkVec3f field_0x2474;
};
static_assert(__builtin_offsetof(UnkMorphBallMovement_l949b7c, field_0x148) == 0x148, "layout");
static_assert(__builtin_offsetof(UnkMorphBallMovement_l949b7c, field_0x2430) == 0x2430, "layout");
static_assert(__builtin_offsetof(UnkMorphBallMovement_l949b7c, field_0x2474) == 0x2474, "layout");

static inline bool IsNear(f32 a, f32 b) {
    f32 eps = g_unk_7101647774;
    return a + eps >= b && a - eps <= b;
}

// (x, y, z) scaled to about unit length (kept finite at zero).
static inline UnkVec3f Direction(f32 x, f32 y, f32 z) {
    return Normalized(UnkVec3f(x, y, z));
}

UnkVec3f fn_7100949b7c(UnkMorphBallMovement_l949b7c* self) {
    UnkVec3f dir(0.0f, 1.0f, 0.0f);
    CVector2D pos = {self->owner->field_0x44, self->owner->field_0x54};
    if (self->field_0x2430) {
        if (self->field_0x2440.Touches(&pos, 0.0f)) {
            dir = self->field_0x2474;
            return dir;
        }
        if (self->field_0x2454.Touches(&pos, 0.0f)) {
            dir = self->field_0x2468;
            return dir;
        }
        const CVector2D* from = &self->field_0x2440.start;
        if (!IsNear(self->field_0x2440.start.x, self->field_0x2454.end.x) ||
            !IsNear(self->field_0x2440.start.y, self->field_0x2454.end.y))
            from = &self->field_0x2440.end;
        CVector2D origin = *from;
        dir = Direction(self->owner->field_0x44 - origin.x, self->owner->field_0x54 - origin.y, 0.0f);
    } else if (self->field_0x128 && self->field_0x128->field_0x10) {
        if (self->field_0xf9)
            dir = Direction(pos.x - self->field_0x13c, pos.y - self->field_0x140, 0.0f);
        else
            dir = UnkVec3f(-self->field_0x148.x, -self->field_0x148.y, -self->field_0x148.z);
    }
    return dir;
}
