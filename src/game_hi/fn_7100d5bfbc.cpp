// Queries a 2D bounding-volume tree and its sub-trees for objects overlapping a shape's bounds,
// filtered by a layer mask; returns whether anything reported a hit.
#include "game/UnkTU2510.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

inline const UnkFlags& NoFlags_d5bfbc() {
    static UnkFlags s;
    return s;
}

struct UnkQueryCtx {
    u32 mask;
    void* arg;
    void* field_0x10;
    UnkFlags result;
    UnkBvhCollider* shape;
    u16 arg2;
};

bool fn_7100d5cea8(UnkQueryCtx* ctx, const UnkNode* node);

bool fn_7100d5bfbc(UnkTree* self, u32 mask, UnkBvhCollider* shape, void* arg, u32 arg2) {
    UnkFlags result = NoFlags_d5bfbc();
    if (shape->boundsDirty)
        shape->UpdateBounds();
    UnkBox bounds = shape->bounds;
    if (self->field_0x5c & mask) {
        UnkQueryCtx ctx = {mask, arg, self->field_0x90, result, shape, static_cast<u16>(arg2)};
        u32 count = self->nodeCount;
        u32 i = 0;
        while (i < count) {
            const UnkNode& n = self->nodes[i];
            if (bounds.max.x >= n.minX && bounds.min.x <= n.maxX && bounds.max.y >= n.minY && bounds.min.y <= n.maxY) {
                if (n.index < 0 && !fn_7100d5cea8(&ctx, &n))
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
                result.bits |= t->Query(result, shape, arg2, mask, &bounds, arg).bits;
                break;
            }
        }
    }
    return result.bits & 1;
}
