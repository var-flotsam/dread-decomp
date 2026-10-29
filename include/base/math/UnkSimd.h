#pragma once

// Four-lane float vectors as the original's hand-vectorised math uses them (the listener's look-at
// matrix, fn_71000f88b8; the frustum update, fn_71001b83f8): a 3D vector lives in a 128-bit
// register with w = 0, horizontal sums are a 64-bit add of the halves and a pairwise add, and
// normalisation is the reciprocal square-root estimate refined by two Newton steps. Written with
// Clang's vector extensions so that every target compiles it; on AArch64 the estimate and the
// steps are the NEON instructions (frsqrte/frsqrts), elsewhere 1/sqrt and its exact step, which
// can differ from the original in the last bits. Names are placeholders.

#include <math.h>

#include "types.h"

#if defined(__aarch64__)
#include <arm_neon.h>
#endif

namespace base::math::simd {

typedef f32 Vec4 __attribute__((ext_vector_type(4)));
typedef f32 Vec2 __attribute__((ext_vector_type(2)));
typedef s32 Mask4 __attribute__((ext_vector_type(4)));

// The two halves of a vector, and a vector from two halves.
__attribute__((always_inline)) inline Vec2 Low(Vec4 v) {
    return __builtin_shufflevector(v, v, 0, 1);
}
__attribute__((always_inline)) inline Vec2 High(Vec4 v) {
    return __builtin_shufflevector(v, v, 2, 3);
}
__attribute__((always_inline)) inline Vec4 Combine(Vec2 lo, Vec2 hi) {
    return __builtin_shufflevector(lo, hi, 0, 1, 2, 3);
}

// Both lanes set to the sum of the two (faddp).
__attribute__((always_inline)) inline Vec2 PairwiseAdd(Vec2 v) {
#if defined(__aarch64__)
    return (Vec2)vpadd_f32((float32x2_t)v, (float32x2_t)v);
#else
    f32 s = v.x + v.y;
    return Vec2{s, s};
#endif
}

// The sum of the four lanes in every lane.
__attribute__((always_inline)) inline Vec4 HorizontalSum(Vec4 v) {
    Vec2 s = PairwiseAdd(High(v) + Low(v));
    return Combine(s, s);
}

// a.b (all four lanes; a 3D vector's w is 0) in every lane.
__attribute__((always_inline)) inline Vec4 Dot(Vec4 a, Vec4 b) {
    return HorizontalSum(a * b);
}

// The reciprocal square root of each lane: the estimate and two Newton steps.
__attribute__((always_inline)) inline Vec4 ReciprocalSqrt(Vec4 x) {
#if defined(__aarch64__)
    float32x4_t vx = (float32x4_t)x;
    float32x4_t e = vrsqrteq_f32(vx);
    e = vmulq_f32(e, vrsqrtsq_f32(e, vmulq_f32(e, vx)));
    e = vmulq_f32(e, vrsqrtsq_f32(e, vmulq_f32(vx, e)));
    return (Vec4)e;
#else
    return Vec4{1.0f / sqrtf(x.x), 1.0f / sqrtf(x.y), 1.0f / sqrtf(x.z), 1.0f / sqrtf(x.w)};
#endif
}

// `v` scaled to unit length, or zero when its length is zero.
__attribute__((always_inline)) inline Vec4 Normalize(Vec4 v) {
    Vec4 length_sq = Dot(v, v);
    Vec4 n = v * ReciprocalSqrt(length_sq);
    return (Vec4)((Mask4)n & (length_sq != 0.0f));
}

// Two lanes of `v`, picked by a table lookup (tbl) as the original does, whatever the lanes.
template <int I, int J>
__attribute__((always_inline)) inline Vec2 Pick(Vec4 v) {
#if defined(__aarch64__)
    static constexpr u8 kIndex[8] = {I * 4, I * 4 + 1, I * 4 + 2, I * 4 + 3, J * 4, J * 4 + 1, J * 4 + 2, J * 4 + 3};
    return (Vec2)vqtbl1_u8((uint8x16_t)v, vld1_u8(kIndex));
#else
    return __builtin_shufflevector(v, v, I, J);
#endif
}

// a x b: the products of the lane orders (y, z, x, w) and (z, x, y, w), each order picked as two
// halves.
__attribute__((always_inline)) inline Vec4 Cross(Vec4 a, Vec4 b) {
    Vec2 a_yz = Pick<1, 2>(a);
    Vec2 a_xw = Pick<0, 3>(a);
    Vec2 b_zx = Pick<2, 0>(b);
    Vec2 b_yw = Pick<1, 3>(b);
    Vec2 a_zx = Pick<2, 0>(a);
    Vec2 a_yw = Pick<1, 3>(a);
    Vec2 b_yz = Pick<1, 2>(b);
    Vec2 b_xw = Pick<0, 3>(b);
#if defined(__aarch64__)
    float32x4_t p = vcombine_f32((float32x2_t)(a_yz * b_zx), (float32x2_t)(a_xw * b_yw));
    return (Vec4)vfmsq_f32(p, (float32x4_t)Combine(a_zx, a_yw), (float32x4_t)Combine(b_yz, b_xw));
#else
    return Combine(a_yz * b_zx, a_xw * b_yw) - Combine(a_zx, a_yw) * Combine(b_yz, b_xw);
#endif
}

// A 3D vector (w = 0), built from its halves.
__attribute__((always_inline)) inline Vec4 Make3(f32 x, f32 y, f32 z) {
    return Combine(Vec2{x, y}, Vec2{z, 0.0f});
}

}  // namespace base::math::simd
