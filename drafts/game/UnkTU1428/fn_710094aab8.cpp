// CMorphBallMovement's unit: snaps a direction to the nearest of four axes in the xy plane (up, right
// and the two diagonals) with the direction's sign along it; the up and right components count only
// beyond 0.995. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMorphBall_aab8;

static inline f32 Dot(const UnkVec3f& a, const UnkVec3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline f32 Sign(f32 v) {
    return static_cast<f32>((v > 0.0f) - (v < 0.0f));
}

static inline UnkVec3f Scaled(const UnkVec3f& v, f32 s) {
    UnkVec3f r;
    r.x = v.x * s;
    r.y = v.y * s;
    r.z = v.z * s;
    return r;
}

// Dots within this of zero snap to zero.
static inline f32 SnapNearAxis(f32 d) {
    return (d < 0.995f && d > -0.995f) ? 0.0f : d;
}

UnkVec3f fn_710094aab8(UnkMorphBall_aab8* self, const UnkVec3f& dir) {
    const f32 kDiagonal = 0x1.6a0358p-1f;  // about 1/sqrt(2)
    const UnkVec3f up(0.0f, 1.0f, 0.0f);
    const UnkVec3f right(1.0f, 0.0f, 0.0f);
    const UnkVec3f diagonal(kDiagonal, kDiagonal, 0.0f);
    const UnkVec3f antidiagonal(-kDiagonal, kDiagonal, 0.0f);

    f32 dot_up = SnapNearAxis(Dot(dir, up));
    f32 dot_right = SnapNearAxis(Dot(dir, right));
    f32 dot_diagonal = Dot(dir, diagonal);
    f32 dot_antidiagonal = Dot(dir, antidiagonal);

    f32 abs_up = __builtin_fabsf(dot_up);
    f32 abs_right = __builtin_fabsf(dot_right);
    f32 abs_diagonal = __builtin_fabsf(dot_diagonal);
    f32 abs_antidiagonal = __builtin_fabsf(dot_antidiagonal);

    f32 best = abs_up < abs_right ? abs_right : abs_up;
    best = best < abs_diagonal ? abs_diagonal : best;
    best = best < abs_antidiagonal ? abs_antidiagonal : best;

    if (abs_up == best)
        return Scaled(up, Sign(dot_up));
    if (abs_right == best)
        return Scaled(right, Sign(dot_right));
    if (abs_diagonal == best)
        return Scaled(diagonal, Sign(dot_diagonal));
    if (abs_antidiagonal == best)
        return Scaled(antidiagonal, Sign(dot_antidiagonal));
    return up;
}
