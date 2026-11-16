// Unit 182: sets a texture builder's component swizzle for a format: one-channel formats repeat red
// (with alpha one, green or alpha), everything else keeps red, green, blue, alpha. Names are
// placeholders.
#include "types.h"

// The swizzle values: zero, one, then the red, green, blue and alpha components.
enum UnkSwizzle_l1604e0 {
    SWIZZLE_ZERO,
    SWIZZLE_ONE,
    SWIZZLE_R,
    SWIZZLE_G,
    SWIZZLE_B,
    SWIZZLE_A,
};

extern void (*g_unk_716e29b128)(void* builder, s32 r, s32 g, s32 b, s32 a);

void fn_71001604e0(void* builder, s32 format) {
    switch (format) {
    case 1:
        g_unk_716e29b128(builder, SWIZZLE_R, SWIZZLE_R, SWIZZLE_R, SWIZZLE_ONE);
        break;
    case 13:
    case 14:
    case 18:
        g_unk_716e29b128(builder, SWIZZLE_R, SWIZZLE_R, SWIZZLE_R, SWIZZLE_G);
        break;
    case 51:
    case 53:
        g_unk_716e29b128(builder, SWIZZLE_R, SWIZZLE_R, SWIZZLE_R, SWIZZLE_A);
        break;
    case 2 ... 12:
    case 15 ... 17:
    case 19 ... 50:
    case 52:
        g_unk_716e29b128(builder, SWIZZLE_R, SWIZZLE_G, SWIZZLE_B, SWIZZLE_A);
        break;
    default:  // an unknown format keeps its components as well
        g_unk_716e29b128(builder, SWIZZLE_R, SWIZZLE_G, SWIZZLE_B, SWIZZLE_A);
        break;
    }
}
