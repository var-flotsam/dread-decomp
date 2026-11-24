// Queries a 2D bounding-volume tree and its sub-trees for objects overlapping a box (given by two
// corners), filtered by a layer mask; returns whether anything reported a hit.
#include "game/UnkTU2510.h"
#include "game/logic/collision/UnkBvhTree.h"
#include "types.h"

struct UnkQueryCtx {
    u32 mask;
    void* arg;
    void* field_0x10;
    UnkFlags result;
    const UnkBox* box;
    const UnkBox* bounds;
};

bool fn_7100d5d0bc(UnkQueryCtx* ctx, const UnkBvhNode* node);

bool fn_7100d5c518(UnkTree* self, u32 mask, const UnkBox* box, void* arg, u32 arg2) {
    f32 x0 = box->min.x, y0 = box->min.y, x1 = box->max.x, y1 = box->max.y;
    UnkBox bounds = {x1 > x0 ? x0 : x1, y1 > y0 ? y0 : y1, x1 < x0 ? x0 : x1, y1 < y0 ? y0 : y1};
    UnkFlags result = 0;
    if (self->field_0x5c & mask) {
        UnkQueryCtx ctx = {mask, arg, self->field_0x90, 0, box, &bounds};
        u32 count = self->nodeCount;
        u32 i = 0;
        while (i < count) {
            const UnkBvhNode& n = self->nodes[i];
            if (bounds.max.x >= n.minX && bounds.min.x <= n.maxX && bounds.max.y >= n.minY && bounds.min.y <= n.maxY) {
                if (n.index < 0 && !fn_7100d5d0bc(&ctx, &n))
                    break;
                ++i;
            } else {
                i = n.index >= 0 ? (u32)n.index : i + 1;
            }
        }
        result = ctx.result;
        if (result.bits & 2)
            return result.bits & 1;
    }
    for (UnkSubTree* t = self->SubBegin(); t != self->SubEnd(); ++t) {
        if ((t->field_0x8 & mask) && !(t->field_0xc & mask)) {
            result.bits |= t->Query(result, box, mask, &bounds, arg, arg2).bits;
            break;
        }
    }
    return result.bits & 1;
}
