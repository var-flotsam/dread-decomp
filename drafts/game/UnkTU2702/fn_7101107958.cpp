// Unit 2702 (hint CAIComponent): the distance along an AI component's path from one point to another,
// walking the segments forwards or backwards (FLT_MAX when an open path cannot be walked that way).
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkPath442.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

// The path the component follows (an UnkPath442).
struct UnkPath_l107958 {
    CVector3D* points;  // 0x0
    u32 count;          // 0x8
    u8 pad_0xc[0x20];
    u8 closed;  // 0x2c

    u32 SegmentCount() const {
        bool open = closed == 0;
        return count - ((count != 0) & open);
    }

    // Segment `i`: from point i to the next one, the last one closing on the first.
    UnkSegment2D Segment(u32 i) const {
        return UnkSegment2D(*reinterpret_cast<const CVector2D*>(&points[i]),
                            *reinterpret_cast<const CVector2D*>(i == count - 1 ? &points[0] : &points[i + 1]));
    }

    u32 Next(u32 i) const { return i == SegmentCount() - 1 ? 0 : i + 1; }
    u32 Prev(u32 i) const { return (i == 0 ? SegmentCount() : i) - 1; }

    f32 Length(u32 i) const { return fn_710035e544(reinterpret_cast<const UnkPath442*>(this), i); }
};

struct UnkPathSelf_l107958 {
    u8 pad_0x0[0x68];
    UnkPath_l107958 path;  // 0x68
};

static inline bool InRange(f32 v, f32 min, f32 max) {
    return v <= max && v >= min;
}

static inline f32 Distance2D(CVector2D a, CVector2D b) {
    return base::math::Length(a - b);
}

f32 fn_7101107958(UnkPathSelf_l107958* self, bool forward, const CVector2D* from, const CVector2D* to, s32 from_seg,
                  s32 to_seg, f32 from_t, f32 to_t) {
    if (from_seg == to_seg) {
        // Already there (within a thousandth).
        if (InRange(to_t, from_t - 0.001f, from_t + 0.001f))
            return 0.0f;
        if (!self->path.closed) {
            if (forward ? from_t > to_t : from_t < to_t)
                return 3.40282347e+38f;
            return Distance2D(*to, *from);
        }
        UnkSegment2D seg = self->path.Segment(from_seg);
        CVector2D dir = seg.end - seg.start;
        CVector2D d = *to - *from;
        CVector2D step = {forward ? d.x : -d.x, forward ? d.y : -d.y};
        if (base::math::Dot(dir, step) > 0)
            return base::math::Length(d);
        // Behind on the same segment: once round the whole loop.
        f32 total = 0.0f;
        for (u32 i = 0; i < self->path.SegmentCount(); i++)
            total += self->path.Length(i);
        return total;
    }

    if (!self->path.closed && (forward ? from_seg > to_seg : from_seg < to_seg))
        return 3.40282347e+38f;

    s32 i = forward ? self->path.Next(from_seg) : self->path.Prev(from_seg);
    UnkSegment2D seg = self->path.Segment(i);
    // Where the walk enters the next segment.
    CVector2D corner = forward ? seg.start : seg.end;
    f32 total = 0.0f;
    total += Distance2D(corner, *from);
    if (i != to_seg) {
        total += self->path.Length(i);
        if (forward) {
            for (;;) {
                i = self->path.Next(i);
                UnkSegment2D next = self->path.Segment(i);
                fn_7100125b44(&seg, &next);
                if (i == to_seg)
                    break;
                total += self->path.Length(i);
            }
        } else {
            for (;;) {
                i = self->path.Prev(i);
                UnkSegment2D next = self->path.Segment(i);
                fn_7100125b44(&seg, &next);
                if (i == to_seg)
                    break;
                total += self->path.Length(i);
            }
        }
        corner = forward ? seg.start : seg.end;
    }
    total += Distance2D(*to, corner);
    return total;
}
