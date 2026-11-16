// Unit 110 (2D geometry): the outline of a box of the given size moved from one point to another:
// four corners when the move is along an axis, else six. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

static const f32 kFloatMax = 3.4028235e+38f;
static const f32 kAxisEpsilon = 0.001f;

static inline CVector2D Vec(f32 x, f32 y) {
    CVector2D v;
    v.x = x;
    v.y = y;
    return v;
}

static inline void Set(CVector2D& p, f32 x, f32 y) {
    p.x = x;
    p.y = y;
}

static inline void Extend(CVector2D& lo, CVector2D& hi, const CVector2D& p) {
    lo.x = lo.x < p.x ? lo.x : p.x;
    lo.y = lo.y < p.y ? lo.y : p.y;
    hi.x = hi.x > p.x ? hi.x : p.x;
    hi.y = hi.y > p.y ? hi.y : p.y;
}

static inline bool IsNearZero(f32 v) {
    return v - kAxisEpsilon <= 0.0f && v + kAxisEpsilon >= 0.0f;
}

void fn_710012d4f0(const CVector2D* size, const CVector2D* a, const CVector2D* b, CVector2D* out, u32* count) {
    f32 hx = size->x * 0.5f;
    f32 hy = size->y * 0.5f;
    CVector2D lo = Vec(kFloatMax, kFloatMax);
    CVector2D hi = Vec(-kFloatMax, -kFloatMax);
    Extend(lo, hi, *a);
    Extend(lo, hi, *b);

    // Every outline ends at its lower left corner.
    u32 n = 0;
    f32 lastX;
    f32 lastY;
    if (IsNearZero(b->x - a->x) || IsNearZero(b->y - a->y)) {
        Set(out[n++], lo.x - hx, hy + hi.y);
        Set(out[n++], hx + hi.x, hy + hi.y);
        Set(out[n++], hx + hi.x, lo.y - hy);
        lastX = lo.x - hx;
        lastY = lo.y - hy;
    } else {
        const CVector2D* left = a->x < b->x ? a : b;
        const CVector2D* right = a->x > b->x ? a : b;
        Set(out[n++], left->x - hx, hy + left->y);
        if (right->y > left->y) {
            Set(out[n++], right->x - hx, hy + right->y);
            Set(out[n++], hx + right->x, hy + right->y);
            Set(out[n++], hx + right->x, right->y - hy);
            Set(out[n++], hx + left->x, left->y - hy);
        } else {
            Set(out[n++], hx + left->x, hy + left->y);
            Set(out[n++], hx + right->x, hy + right->y);
            Set(out[n++], hx + right->x, right->y - hy);
            Set(out[n++], right->x - hx, right->y - hy);
        }
        lastX = left->x - hx;
        lastY = left->y - hy;
    }
    Set(out[n++], lastX, lastY);
    *count = n;
}
