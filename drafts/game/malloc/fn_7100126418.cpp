// malloc unit: closest points between two bounded lines (each a direction and a half-length) in the
// plane. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector2D;

void fn_7100126418(CVector2D* gap, CVector2D* pointA, CVector2D* pointB, f32* sA, f32* sB, const CVector2D* offset,
                   const CVector2D* dirA, const CVector2D* dirB, f32 halfA, f32 halfB) {
    f32 cosine = dirA->x * dirB->x + dirA->y * dirB->y;
    f32 projA = dirA->x * offset->x + dirA->y * offset->y;
    f32 projB = dirB->x * offset->x + dirB->y * offset->y;
    f32 det = 1.0f - cosine * cosine;
    f32 s;
    if (det == 0.0f) {
        s = 0.0f;
        *sA = s;
    } else {
        s = (projA - cosine * projB) / det;
        *sA = s;
        if (s < -halfA) {
            s = -halfA;
            *sA = s;
        } else if (s > halfA) {
            s = halfA;
            *sA = s;
        }
    }
    f32 t = cosine * s - projB;
    *sB = t;
    if (t < -halfB) {
        *sB = -halfB;
        s = cosine * -halfB + projA;
        *sA = s;
        if (s < -halfA) {
            *sA = -halfA;
            s = -halfA;
        } else if (s > halfA) {
            s = halfA;
            *sA = halfA;
        }
    } else if (t > halfB) {
        *sB = halfB;
        s = cosine * halfB + projA;
        *sA = s;
        if (s < -halfA) {
            *sA = -halfA;
            s = -halfA;
        } else if (s > halfA) {
            s = halfA;
            *sA = halfA;
        }
    } else {
        s = *sA;
    }
    CVector2D da = *dirA;
    pointA->x = s * da.x;
    pointA->y = s * da.y;
    CVector2D db = *dirB;
    f32 t2 = *sB;
    f32 bx = t2 * db.x;
    f32 by = t2 * db.y;
    pointB->x = bx;
    pointB->y = by;
    CVector2D off = *offset;
    CVector2D pa = *pointA;
    gap->y = (off.y - pa.y) + by;
    gap->x = (off.x - pa.x) + bx;
}
