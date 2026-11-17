// Unit 2702 (hint CAIComponent): an AI component's distance along x from its owner to the path's
// last segment's end when its transform's m[2] is positive, else to its first point.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkPathOwner_l0fc68c {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
};

struct UnkPathPoint_l0fc68c {
    f32 x, y, z;
};

struct UnkPathSelf_l0fc68c {
    u8 pad_0x0[0x30];
    UnkPathOwner_l0fc68c* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkPathPoint_l0fc68c* points;  // 0x68
    u32 count;                 // 0x70
    u8 pad_0x74[0x20];
    u8 closed;  // 0x94

    // The path's points other than a closing one.
    u32 Open() const {
        bool open = closed == 0;
        return (count != 0) & open;
    }
};

static inline const base::math::CVector2D& Pt_l0fc68c(const UnkPathPoint_l0fc68c* p) {
    return *reinterpret_cast<const base::math::CVector2D*>(p);
}

// x of the path's first point.
static inline f32 FirstX_l0fc68c(const UnkPathSelf_l0fc68c* self) {
    UnkPathPoint_l0fc68c* points = self->points;
    UnkSegment2D seg(Pt_l0fc68c(points), Pt_l0fc68c(self->count == 1 ? points : points + 1));
    return seg.start.x;
}

// x of the end of the path's last segment, 0 when there is none.
static inline f32 LastX_l0fc68c(const UnkPathSelf_l0fc68c* self) {
    u32 n = self->count - self->Open();
    if (n == 0)
        return 0.0f;
    u32 last = n - 1;
    UnkPathPoint_l0fc68c* points = self->points;
    UnkSegment2D seg(Pt_l0fc68c(&points[last]), Pt_l0fc68c(self->Open() ? &points[n] : &points[0]));
    return seg.end.x;
}

bool fn_71010fc68c(UnkGoliathAI_b58a3* ai, f32* out) {
    UnkPathSelf_l0fc68c* self = reinterpret_cast<UnkPathSelf_l0fc68c*>(ai);
    const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
    f32 x;
    if (m.m[2] > 0) {
        if (self->count == self->Open())
            return false;
        x = LastX_l0fc68c(self);
    } else {
        if (self->count == self->Open())
            return false;
        x = FirstX_l0fc68c(self);
    }
    *out = __builtin_fabsf(self->owner->transform.m[3] - x);
    return true;
}
