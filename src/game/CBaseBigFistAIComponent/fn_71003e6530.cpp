// Samples a quarter circle of probe points around an origin: starting straight above it at the
// given radius, steps the angle in nine-degree increments (the sines and cosines are constants),
// handing each point to fn_7100118b94 until a point lies further out than the limit, where the
// run ends with a point on the limit line (and a last one level with the origin). The sign of the
// direction's y picks the side. The unused first parameter is the owner.
#include "types.h"

struct UnkVec2 {
    f32 x;
    f32 y;
};

struct UnkVec3 {
    f32 x;
    f32 y;
    f32 z;
};

void* fn_7100118b94(void* sink, UnkVec3* point);

// One step: the point at (radius * a, radius * b) from the origin, mirrored by the direction's sign.
#define STEP(A, B)                                                                       \
    {                                                                                    \
        f32 ax = radius * (A);                                                           \
        f32 ay = radius * (B);                                                           \
        f32 sign = (f32)((dir->y > 0.0f) - (dir->y < 0.0f));                             \
        if (ax > limit) {                                                                \
            tail_y = ay;                                                                 \
            tail_sign = sign;                                                            \
            break;                                                                       \
        }                                                                                \
        f32 scaled = ax * sign;                                                          \
        f32 ox = origin->x;                                                              \
        f32 oy = origin->y;                                                              \
        point.z = 0.0f;                                                                  \
        f32 nx = scaled + ox;                                                            \
        f32 ny = ay + oy;                                                                \
        point.x = nx;                                                                    \
        point.y = ny;                                                                    \
        fn_7100118b94(sink, &point);                                                     \
    }

// NON_MATCHING(minor): the first step adds and multiplies in the other order, and the shared tail
// keeps the point pointer in x1 and stores the zero z through it before the call (scheduling only)
void* fn_71003e6530(void* owner, const UnkVec2* origin, const UnkVec2* dir, void* sink, f32 radius, f32 limit) {
    f32 tail_y;
    f32 tail_sign;
    UnkVec3 point;
    do {
        {
            f32 ox = origin->x;
            f32 oy = origin->y;
            point.z = 0.0f;
            f32 ny = oy + radius;
            f32 zero_term = radius * 0.0f;
            f32 nx = zero_term + ox;
            point.x = nx;
            point.y = ny;
            fn_7100118b94(sink, &point);
        }
        STEP(0x1.4060aap-3f, 0x1.f9b24ap-1f)
        STEP(0x1.3c6ef2p-2f, 0x1.e6f0e2p-1f)
        STEP(0x1.d0e2e4p-2f, 0x1.c83202p-1f)
        STEP(0x1.2cf232p-1f, 0x1.9e377ap-1f)
        STEP(0x1.6a09e8p-1f, 0x1.6a09e6p-1f)
        STEP(0x1.9e377ap-1f, 0x1.2cf230p-1f)
        STEP(0x1.c83202p-1f, 0x1.d0e2e0p-2f)
        STEP(0x1.e6f0e2p-1f, 0x1.3c6ef0p-2f)
        STEP(0x1.f9b24ap-1f, 0x1.4060b0p-3f)
        {
            f32 ay = radius * -0x1.0p-24f;
            f32 sign = (f32)((dir->y > 0.0f) - (dir->y < 0.0f));
            if (radius > limit) {
                tail_y = ay;
                tail_sign = sign;
                break;
            }
            f32 scaled = sign * radius;
            f32 ox = origin->x;
            f32 oy = origin->y;
            point.z = 0.0f;
            f32 nx = scaled + ox;
            f32 ny = ay + oy;
            point.x = nx;
            point.y = ny;
            fn_7100118b94(sink, &point);
            UnkVec2 o2 = *origin;
            f32 side = (f32)((dir->y > 0.0f) - (dir->y < 0.0f)) * radius;
            f32 zero_term = side * 0.0f;
            point.x = o2.x + side;
            point.y = o2.y + zero_term;
            goto done;
        }
    } while (0);
    {
        f32 side = tail_sign * limit;
        f32 ox = origin->x;
        f32 oy = origin->y;
        point.z = 0.0f;
        f32 ny = tail_y + oy;
        f32 nx = side + ox;
        point.x = nx;
        point.y = ny;
        fn_7100118b94(sink, &point);
        f32 ox2 = origin->x;
        u32 oy_bits = *reinterpret_cast<const u32*>(&origin->y);
        point.x = side + ox2;
        *reinterpret_cast<u32*>(&point.y) = oy_bits;
    }
done:
    point.z = 0.0f;
    return fn_7100118b94(sink, &point);
}
