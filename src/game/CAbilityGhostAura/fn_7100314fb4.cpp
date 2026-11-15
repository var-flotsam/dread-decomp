// CAbilityGhostAura unit: orders two targets by their planar distance to the context's point (-1, 0 or 1);
// on a tie, when the context has a direction and both targets are flagged (bit 4 at 0x3c), by who faces it
// more. Names and layouts are placeholders.
#include "types.h"

struct UnkGhostAuraDir_l314fb4 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkGhostAuraSortCtx_b0040 {
    f32 x;
    f32 y;
    f32 z;
    u8 pad_0xc[4];
    const UnkGhostAuraDir_l314fb4* direction;
};

struct UnkGhostAuraTargetA_b0040 {
    u8 pad_0x0[0x14];
    f32 pos_x;
    f32 pos_y;
    u8 pad_0x1c[0x2c - 0x1c];
    UnkGhostAuraDir_l314fb4 normal;
    u8 pad_0x38[4];
    u8 flags;
};

static inline f32 DistSq_l314fb4(const UnkGhostAuraSortCtx_b0040* ctx, const UnkGhostAuraTargetA_b0040* t) {
    f32 dz = ctx->z;
    f32 dy = ctx->y - t->pos_y;
    f32 dx = ctx->x - t->pos_x;
    return dz * dz + (dx * dx + dy * dy);
}

static inline f32 Dot_l314fb4(const UnkGhostAuraDir_l314fb4* d, const UnkGhostAuraTargetA_b0040* t) {
    return t->normal.x * d->x + t->normal.y * d->y + t->normal.z * d->z;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
s32 fn_7100314fb4(const UnkGhostAuraSortCtx_b0040* ctx, const UnkGhostAuraTargetA_b0040* a,
                  const UnkGhostAuraTargetA_b0040* b) {
    f32 dist_b = DistSq_l314fb4(ctx, b);
    f32 dist_a = DistSq_l314fb4(ctx, a);
    f32 diff = dist_b - dist_a;
    f32 order = (diff > 0.0f) - (diff < 0.0f);
    s32 result = static_cast<s32>(order);
    const UnkGhostAuraDir_l314fb4* direction = ctx->direction;
    if (direction == nullptr || result != 0)
        return result;
    if (!(b->flags & 0x10))
        return 0;
    if (!(a->flags & 0x10))
        return 0;
    return Dot_l314fb4(direction, a) > Dot_l314fb4(direction, b);
}
