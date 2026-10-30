// Queries a 2D bounding-volume tree and its sub-trees for objects overlapping a shape placed with
// an extra transform, filtered by a layer mask; returns whether anything reported a hit.
#include "types.h"

struct UnkFlags {
    u32 bits;
    UnkFlags() : bits(0) {}
    UnkFlags(const UnkFlags& o) : bits(o.bits) {}
};

inline const UnkFlags& NoFlags_d5b484() {
    static UnkFlags s;
    return s;
}

struct Vec3 {
    f32 x, y, z;
};

struct Vec2 {
    f32 x, y;
    Vec2(const Vec3& v) asm("fn_710007b430");  // fn_710007b430
};

struct UnkAABB3 {
    Vec3 min, max;
};

struct UnkBox {
    Vec2 min, max;
    UnkBox(const UnkAABB3& b) : min(b.min), max(b.max) {}
};

struct UnkMtx44 {
    f32 m[16];
    UnkMtx44(const UnkMtx44& o) {
        for (int i = 0; i < 16; ++i)
            m[i] = o.m[i];
    }
};

UnkMtx44 operator*(const UnkMtx44& a, const UnkMtx44& b);  // fn_71000127c8

struct UnkShape {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkAABB3 vf_0x28(const UnkMtx44& xf);  // bounds under a transform
    UnkMtx44 transform;
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
    UnkMtx44 transform;
    const UnkBox* bounds;
    u16 arg2;
};

struct UnkSubTree {
    char pad[8];
    u32 field_0x8;
    u32 field_0xc;
    char pad2[0x90 - 0x10];
    UnkFlags Query(UnkFlags flags, const UnkMtx44* xf, u16 arg2, UnkShape* shape, u32 mask, const UnkBox* bounds, void* arg);  // fn_7100d5c370
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

bool fn_7100d5cfb4(UnkQueryCtx* ctx, const UnkNode* node);

bool fn_7100d5b484(UnkTree* self, u32 mask, UnkShape* shape, const UnkMtx44* xf, void* arg, u16 arg2) {
    UnkFlags result = NoFlags_d5b484();
    UnkMtx44 m = *xf * shape->transform;
    UnkBox bounds = shape->vf_0x28(m);
    if (self->field_0x5c & mask) {
        UnkQueryCtx ctx = {mask, arg, self->field_0x90, result, shape, m, &bounds, arg2};
        u32 count = self->nodeCount;
        u32 i = 0;
        while (i < count) {
            const UnkNode& n = self->nodes[i];
            if (bounds.max.x >= n.minX && bounds.min.x <= n.maxX && bounds.max.y >= n.minY && bounds.min.y <= n.maxY) {
                if (n.index < 0 && !fn_7100d5cfb4(&ctx, &n))
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
                result.bits |= t->Query(result, &m, arg2, shape, mask, &bounds, arg).bits;
                break;
            }
        }
    }
    return result.bits & 1;
}
