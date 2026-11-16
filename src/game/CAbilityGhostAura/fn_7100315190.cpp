// CAbilityGhostAura unit: orders two targets by their planar distance to the context's point (-1, 0 or 1);
// on a tie, when the context has a direction and both targets are flagged (bit 4 at 0x2c), by who faces it
// more. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "types.h"

using base::math::CVector3D;

struct UnkGhostAuraSortCtx_l315190 {
    f32 x;
    f32 y;
    f32 z;
    u8 pad_0xc[4];
    const CVector3D* direction;
};

struct UnkGhostAuraTargetA_l315190 {
    u8 pad_0x0[4];
    f32 pos_x;
    f32 pos_y;
    u8 pad_0xc[0x1c - 0xc];
    CVector3D normal;
    u8 pad_0x28[4];
    u8 flags;
};

static inline f32 DistSq_l315190(const UnkGhostAuraSortCtx_l315190* ctx, f32 pos_x, f32 pos_y) {
    f32 dx = ctx->x - pos_x;
    f32 dy = ctx->y - pos_y;
    f32 dz = ctx->z;
    return dz * dz + (dx * dx + dy * dy);
}

static inline f32 Dot_l315190(const CVector3D* d, const UnkGhostAuraTargetA_l315190* t) {
    return t->normal.x * d->x + t->normal.y * d->y + t->normal.z * d->z;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
s32 fn_7100315190(const UnkGhostAuraSortCtx_l315190* ctx, const UnkGhostAuraTargetA_l315190* a,
                  const UnkGhostAuraTargetA_l315190* b) {
    f32 b_x = b->pos_x;
    f32 b_y = b->pos_y;
    f32 dist_b = DistSq_l315190(ctx, b_x, b_y);
    f32 dist_a = DistSq_l315190(ctx, a->pos_x, a->pos_y);
    f32 diff = dist_b - dist_a;
    f32 order = (diff > 0.0f) - (diff < 0.0f);
    s32 result = static_cast<s32>(order);
    const CVector3D* direction = ctx->direction;
    if (direction == nullptr || result != 0)
        return result;
    if (!(b->flags & 0x10))
        return 0;
    if (!(a->flags & 0x10))
        return 0;
    return Dot_l315190(direction, a) > Dot_l315190(direction, b);
}
