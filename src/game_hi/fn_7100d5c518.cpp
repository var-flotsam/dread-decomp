// Queries a 2D bounding-volume tree and its sub-trees for objects overlapping a box (given by two
// corners), filtered by a layer mask; returns whether anything reported a hit.
#include "types.h"

struct UnkFlags {
    u32 bits;
    UnkFlags(u32 b) : bits(b) {}
    UnkFlags(const UnkFlags& o) : bits(o.bits) {}
};

struct UnkBox {
    f32 minX, minY, maxX, maxY;
};

struct UnkNode {
    f32 minX, minY, maxX, maxY;
    s32 index;  // >= 0: index of the next node outside this subtree; < 0: a leaf
};

struct UnkQueryCtx {
    u32 mask;
    void* arg;
    void* field_0x10;
    UnkFlags result;
    const UnkBox* box;
    const UnkBox* bounds;
};

struct UnkSubTree {
    char pad[8];
    u32 field_0x8;
    u32 field_0xc;
    char pad2[0x90 - 0x10];
    UnkFlags Query(UnkFlags flags, const UnkBox* box, u32 mask, const UnkBox* bounds, void* arg, u32 arg2);  // fn_7100d5c6dc
};

struct UnkTree {
    char pad[0x5c];
    u32 field_0x5c;
    UnkNode* nodes;
    u32 nodeCount;
    char pad2[0x90 - 0x6c];
    char field_0x90[0xb8 - 0x90];
    u32 subTreeCount;
    u8 subTrees[4 * sizeof(UnkSubTree)];
    UnkSubTree* SubBegin() { return reinterpret_cast<UnkSubTree*>(subTrees); }
    UnkSubTree* SubEnd() { return SubBegin() + subTreeCount; }
};

bool fn_7100d5d0bc(UnkQueryCtx* ctx, const UnkNode* node);

bool fn_7100d5c518(UnkTree* self, u32 mask, const UnkBox* box, void* arg, u32 arg2) {
    f32 x0 = box->minX, y0 = box->minY, x1 = box->maxX, y1 = box->maxY;
    UnkBox bounds = {x1 > x0 ? x0 : x1, y1 > y0 ? y0 : y1, x1 < x0 ? x0 : x1, y1 < y0 ? y0 : y1};
    UnkFlags result = 0;
    if (self->field_0x5c & mask) {
        UnkQueryCtx ctx = {mask, arg, self->field_0x90, 0, box, &bounds};
        u32 count = self->nodeCount;
        u32 i = 0;
        while (i < count) {
            const UnkNode& n = self->nodes[i];
            if (bounds.maxX >= n.minX && bounds.minX <= n.maxX && bounds.maxY >= n.minY && bounds.minY <= n.maxY) {
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
