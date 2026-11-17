// Unit 2177 of tools/tu_map.py: decides on which side of a path a mover passes, walking the path's edges
// while they keep pointing along its first axis, and sets one of two out-flags.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/UnkSegment2D.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;

// The 2D tests' tolerance (0.001).
extern const f32 g_unk_7101647774;

struct UnkPathVertex_lc70980 {
    CVector2D pos;
    f32 field_0x8;
};

struct UnkPath2D_lc70980 {
    UnkPathVertex_lc70980* vertices;
    u32 count;
    u8 pad_0xc[0x20];
    bool closed;

    u32 EdgeCount() const { return !closed && count != 0 ? count - 1 : count; }
    // The edge after edge i, wrapping to the first after the last.
    u32 Next(u32 i) const { return i == EdgeCount() - 1 ? 0 : i + 1; }
    // The edge from vertex i to the next one (the first after the last).
    UnkSegment2D Edge(u32 i) const {
        UnkPathVertex_lc70980* v = vertices;
        const UnkPathVertex_lc70980* next = count - 1 == i ? v : &v[i + 1];
        return UnkSegment2D(v[i].pos, next->pos);
    }
};
static_assert(__builtin_offsetof(UnkPath2D_lc70980, closed) == 0x2c, "UnkPath2D_lc70980 layout");

struct UnkSideMover_lc70980 {
    u8 pad_0x0[0x150];
    CVector2D pos;
    CVector2D axis_x;
    CVector2D axis_y;
};

static inline bool Near(f32 x, f32 target, f32 eps) {
    f32 lo = x - eps;
    f32 hi = x + eps;
    return lo <= target && hi >= target;
}

static inline bool NearPoint(const CVector2D& a, const CVector2D& b) {
    f32 tol = g_unk_7101647774;
    return a.x - tol <= b.x && a.x + tol >= b.x && a.y - tol <= b.y && a.y + tol >= b.y;
}

// x rounded to a millionth near +-1, to a thousandth elsewhere.
static inline f32 Quantize(f32 x) {
    bool fine = x > 0.97f || x < -0.97f;
    f32 scaled = x * (fine ? 1000000.0f : 1000.0f);
    f32 rounded = scaled >= 0.0f ? floorf(scaled + 0.5f) : ceilf(scaled - 0.5f);
    return rounded * (fine ? 0.000001f : 0.001f);
}

// The direction from the mover to the segment's end, measured along its two axes.
static inline void EdgeAxes(const UnkSideMover_lc70980* self, const UnkSegment2D& seg, f32& along, f32& side) {
    f32 dx = seg.end.x - self->pos.x;
    f32 dy = seg.end.y - self->pos.y;
    f32 len = sqrtf(dx * dx + dy * dy);
    if (len != 0.0f) {
        dx = dx / len;
        dy = dy / len;
    }
    along = Quantize(dx * self->axis_x.x + dy * self->axis_x.y);
    side = Quantize(dx * self->axis_y.x + dy * self->axis_y.y);
}

static inline bool Hit(bool* out) {
    *out = true;
    return true;
}

bool fn_7100c70980(UnkSideMover_lc70980* self, bool reversed, UnkPath2D_lc70980* path, u32 index, bool* out_a,
                   bool* out_b, f32 v, f32 a, f32 b, f32 c, f32 d, f32 e, f32 f, f32 g, f32 h, f32 k) {
    if (v != a && v != b) {
        if (g <= k)
            return Hit(out_a);
        if (h <= k)
            return Hit(out_b);
        if (reversed)
            return Hit(out_a);
        return Hit(out_b);
    }
    if (reversed) {
        if (Near(v, -1.0f, 0.0001f)) {
            bool d_near = Near(d, -1.0f, 0.0001f);
            if (!Near(c, -1.0f, 0.0001f)) {
                if (e > 0.0f)
                    return Hit(out_a);
                return Hit(out_b);
            }
            if (!d_near) {
                if (f < 0.0f)
                    return Hit(out_a);
                return Hit(out_b);
            }
            // Follow the path from the edge after `index` while its edges point along the first axis,
            // until it comes back to where it started; the side comes from the last edge's second axis.
            u32 i = path->Next(index);
            UnkSegment2D first = path->Edge(i);
            i = path->Next(i);
            UnkSegment2D seg = path->Edge(i);
            f32 along, side;
            EdgeAxes(self, seg, along, side);
            while (Near(along, -1.0f, 0.000001f) && !NearPoint(first.start, seg.start)) {
                i = path->Next(i);
                UnkSegment2D next = path->Edge(i);
                fn_7100125b44(&seg, &next);
                EdgeAxes(self, seg, along, side);
            }
            return Hit(side > 0.0f ? out_a : out_b);
        }
        if (c < v && d < v)
            return false;
        bool both = e >= 0.0f && f >= 0.0f;
        if (c >= v) {
            if (both) {
                if (c >= d)
                    return Hit(out_a);
                return Hit(out_b);
            }
            if (e >= 0.0f)
                return Hit(out_a);
            return Hit(out_b);
        }
        if (both) {
            if (f >= e)
                return Hit(out_b);
            return Hit(out_a);
        }
        if (f >= 0.0f)
            return Hit(out_b);
        return Hit(out_a);
    }
    if (Near(v, 1.0f, 0.0001f)) {
        bool c_near = Near(c, 1.0f, 0.0001f);
        if (c_near && Near(d, 1.0f, 0.0001f)) {
            // Follow the path from the edge after `index` while its edges point along the first axis,
            // until it comes back to where it started; the side comes from the last edge's second axis.
            u32 i = path->Next(index);
            UnkSegment2D first = path->Edge(i);
            i = path->Next(i);
            UnkSegment2D seg = path->Edge(i);
            f32 along, side;
            EdgeAxes(self, seg, along, side);
            while (Near(along, 1.0f, 0.000001f) && !NearPoint(first.start, seg.start)) {
                i = path->Next(i);
                UnkSegment2D next = path->Edge(i);
                fn_7100125b44(&seg, &next);
                EdgeAxes(self, seg, along, side);
            }
            return Hit(side > 0.0f ? out_a : out_b);
        }
        if (Near(d, 1.0f, 0.0001f)) {
            if (e > 0.0f)
                return Hit(out_a);
            return Hit(out_b);
        }
        if (c_near) {
            if (f < 0.0f)
                return Hit(out_a);
            return Hit(out_b);
        }
        if (e < 0.0f)
            return Hit(out_b);
        return Hit(out_a);
    }
    if (c > v && d > v)
        return false;
    bool both = e >= 0.0f && f >= 0.0f;
    if (c <= v) {
        if (both) {
            if (f >= e)
                return Hit(out_b);
            return Hit(out_a);
        }
        if (e >= 0.0f)
            return Hit(out_a);
        return Hit(out_b);
    }
    if (both) {
        if (f >= e)
            return Hit(out_b);
        return Hit(out_a);
    }
    if (f >= 0.0f)
        return Hit(out_b);
    return Hit(out_a);
}
