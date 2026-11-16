// malloc unit: closest points between two bounded lines in the plane (each a unit direction and a
// half-length, the second offset from the first): the two parameters, the two points and the gap
// between them. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

static inline CVector2D operator*(f32 s, const CVector2D& v) {
    CVector2D r;
    r.x = s * v.x;
    r.y = s * v.y;
    return r;
}

static inline CVector2D operator+(const CVector2D& a, const CVector2D& b) {
    CVector2D r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    return r;
}

static inline CVector2D operator-(const CVector2D& a, const CVector2D& b) {
    CVector2D r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    return r;
}

// Clamps *value to [-limit, limit].
static inline void ClampToRange(f32* value, f32 limit) {
    if (*value < -limit)
        *value = -limit;
    else if (*value > limit)
        *value = limit;
}

void fn_7100126418(CVector2D* gap, CVector2D* pointA, CVector2D* pointB, f32* sA, f32* sB, const CVector2D* offset,
                   const CVector2D* dirA, const CVector2D* dirB, f32 halfA, f32 halfB) {
    f32 cosine = dirA->x * dirB->x + dirA->y * dirB->y;
    f32 projA = dirA->x * offset->x + dirA->y * offset->y;
    f32 projB = dirB->x * offset->x + dirB->y * offset->y;
    f32 det = 1.0f - cosine * cosine;
    if (det == 0.0f) {
        *sA = 0.0f;
    } else {
        *sA = (projA - cosine * projB) / det;
        ClampToRange(sA, halfA);
    }
    *sB = cosine * *sA - projB;
    if (*sB < -halfB) {
        *sB = -halfB;
        *sA = cosine * *sB + projA;
        ClampToRange(sA, halfA);
    } else if (*sB > halfB) {
        *sB = halfB;
        *sA = cosine * *sB + projA;
        ClampToRange(sA, halfA);
    }
    *pointA = *sA * *dirA;
    CVector2D b = *sB * *dirB;
    *pointB = b;
    *gap = *offset - *pointA + b;
}
