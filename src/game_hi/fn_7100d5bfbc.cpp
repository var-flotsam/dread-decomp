// Queries a 2D bounding-volume tree and its sub-trees for objects overlapping a shape's bounds,
// filtered by a layer mask; returns whether anything reported a hit.
#include "types.h"

struct UnkFlags {
    u32 bits;
    UnkFlags() : bits(0) {}
    UnkFlags(const UnkFlags& o) : bits(o.bits) {}
};

inline const UnkFlags& NoFlags_d5bfbc() {
    static UnkFlags s;
    return s;
}

struct UnkBox {
    f32 minX, minY, maxX, maxY;
    UnkBox(const UnkBox& o) : minX(o.minX), minY(o.minY), maxX(o.maxX), maxY(o.maxY) {}
};

struct UnkShape {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();  // refreshes the cached bounds
    char pad[0x40 - 8];
    UnkBox bounds;
    char pad2[0x60 - 0x50];
    bool boundsDirty;
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
    UnkShape* shape;
    u16 arg2;
};

struct UnkSubTree {
    char pad[8];
    u32 field_0x8;
    u32 field_0xc;
    char pad2[0x90 - 0x10];
    UnkFlags Query(UnkFlags flags, UnkShape* shape, u16 arg2, u32 mask, const UnkBox* bounds, void* arg);  // fn_7100d5c1d0
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

bool fn_7100d5cea8(UnkQueryCtx* ctx, const UnkNode* node);

bool fn_7100d5bfbc(UnkTree* self, u32 mask, UnkShape* shape, void* arg, u16 arg2) {
    UnkFlags result = NoFlags_d5bfbc();
    if (shape->boundsDirty)
        shape->vf_0x98();
    UnkBox bounds = shape->bounds;
    if (self->field_0x5c & mask) {
        UnkQueryCtx ctx = {mask, arg, self->field_0x90, result, shape, arg2};
        u32 count = self->nodeCount;
        u32 i = 0;
        while (i < count) {
            const UnkNode& n = self->nodes[i];
            if (bounds.maxX >= n.minX && bounds.minX <= n.maxX && bounds.maxY >= n.minY && bounds.minY <= n.maxY) {
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
