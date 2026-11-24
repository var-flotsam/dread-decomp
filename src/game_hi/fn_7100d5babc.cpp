// Queries a 2D bounding-volume tree and its sub-trees for objects overlapping a shape swept by a
// 2D offset (the union of its bounds before and after the move), filtered by a layer mask;
// returns whether anything reported a hit.
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

struct UnkQueryCtx {
    u32 mask;
    void* arg;
    void* field_0x10;
    UnkFlags result;
    UnkShape* shape;
    UnkMtx44 transform;
    Vec2 delta;
    const UnkBox* bounds;
};

bool fn_7100d5cd9c(UnkQueryCtx* ctx, const UnkBvhNode* node);

bool fn_7100d5babc(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, const Vec2* delta, void* arg, bool flag) {
    UnkMtx44 m = *xf * shape->transform;
    UnkMtx44 moved = m;
    Vec3 offset = {delta->x, delta->y, 0.0f};
    moved.m[3] = offset.x + m.m[3];
    moved.m[7] = offset.y + m.m[7];
    moved.m[11] = offset.z + m.m[11];
    UnkBox bounds = shape->vf_0x28(m);
    UnkBox after = shape->vf_0x28(moved);
    bounds.min.x = bounds.min.x < after.min.x ? bounds.min.x : after.min.x;
    bounds.min.y = bounds.min.y < after.min.y ? bounds.min.y : after.min.y;
    bounds.max.x = bounds.max.x > after.max.x ? bounds.max.x : after.max.x;
    bounds.max.y = bounds.max.y > after.max.y ? bounds.max.y : after.max.y;
    UnkFlags result = 0;
    if (self->field_0x5c & mask) {
        UnkQueryCtx ctx = {mask, arg, self->field_0x90, result, shape, m, *delta, &bounds};
        u32 count = self->nodeCount;
        u32 i = 0;
        while (i < count) {
            const UnkBvhNode& n = self->nodes[i];
            if (bounds.max.x >= n.minX && bounds.min.x <= n.maxX && bounds.max.y >= n.minY && bounds.min.y <= n.maxY) {
                if (n.index < 0 && !fn_7100d5cd9c(&ctx, &n))
                    break;
                ++i;
            } else {
                i = n.index >= 0 ? (u32)n.index : i + 1;
            }
        }
        result = ctx.result;
    }
    if (!(result.bits & 2)) {
        for (UnkSubTree* t = self->SubBegin(); t != self->SubEnd(); ++t) {
            if ((t->field_0x8 & mask) && !(t->field_0xc & mask)) {
                result.bits |= t->Query(result, &m, delta, shape, mask, &bounds, arg, flag).bits;
                break;
            }
        }
    }
    return result.bits & 1;
}
