#pragma once

// The collision world's 2D bounding-volume tree (tools/tu_map.py unit 2510) and the types its queries pass
// around: the tree (UnkTree), its up to four sub-trees (UnkSubTree, each a plain list of colliders from +0x10),
// its nodes, the query result bits (UnkFlags), the 2D boxes, the shape a query places with a transform
// (UnkShape) and the collider a query tests (UnkBvhCollider). Only what the hand-written queries touch is
// spelled out. Names and layouts are placeholders.
#include "types.h"

// A query's result: bit 0 a hit, bit 1 stop. Returned in memory: its copy is user-written.
struct UnkFlags {
    u32 bits;
    UnkFlags() : bits(0) {}
    UnkFlags(u32 b) : bits(b) {}
    UnkFlags(const UnkFlags& o) : bits(o.bits) {}
};

struct Vec3 {
    f32 x, y, z;
};

struct Vec2 {
    f32 x, y;
    Vec2(f32 x_, f32 y_) : x(x_), y(y_) {}
    Vec2(const Vec3& v);  // fn_710007b430
    Vec2(const Vec2& o) : x(o.x), y(o.y) {}
};

struct UnkAABB3 {
    Vec3 min, max;
};

// A 2D box: its lower and upper corners.
struct UnkBox {
    Vec2 min, max;
    UnkBox(f32 x0, f32 y0, f32 x1, f32 y1) : min(x0, y0), max(x1, y1) {}
    UnkBox(const UnkAABB3& b) : min(b.min), max(b.max) {}
    UnkBox(const UnkBox& o) : min(o.min), max(o.max) {}
};

struct UnkMtx44 {
    f32 m[16];
    UnkMtx44(const UnkMtx44& o) {
        for (int i = 0; i < 16; ++i)
            m[i] = o.m[i];
    }
};

UnkMtx44 operator*(const UnkMtx44& a, const UnkMtx44& b);  // fn_71000127c8

// A collision shape: its type (the pair handlers dispatch on the sum of two), its bounds under a transform,
// and its own transform.
struct UnkShape {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual u32 GetType();  // vf_0x18
    virtual void vf_0x20();
    virtual UnkAABB3 vf_0x28(const UnkMtx44& xf);  // bounds under a transform
    UnkMtx44 transform;
};

// A collider (game::logic::collision::CCollider) as the queries see it: its layers, its cached bounds and
// whether they need refreshing, and whether it takes part.
struct UnkBvhCollider {
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
    virtual u32 GetLayers();  // vf_0x58
    virtual void vf_0x60();
    virtual bool IsSame(const void* other);  // vf_0x68
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void UpdateBounds();  // vf_0x98: refreshes the cached bounds
    u8 pad_0x8[0x38];
    UnkBox bounds;  // 0x40
    u8 pad_0x50[0x10];
    bool boundsDirty;  // 0x60
    u8 pad_0x61[0x9];
    bool enabled;  // 0x6a
};
static_assert(__builtin_offsetof(UnkBvhCollider, enabled) == 0x6a, "UnkBvhCollider layout");

struct UnkNode {
    f32 minX, minY, maxX, maxY;
    s32 index;  // >= 0: index of the next node outside this subtree; < 0: a leaf
};

struct UnkBvhEntry {
    u8 pad_0x0[0x10];
    UnkBvhCollider* collider;  // 0x10
};

// A sub-tree: from +0x10, a plain list of colliders that the queries walk one by one.
struct UnkSubTree {
    char pad[8];
    u32 field_0x8;
    u32 field_0xc;
    UnkBvhEntry** entries;  // 0x10
    u32 count;              // 0x18
    char pad2[0x90 - 0x1c];

    UnkBvhEntry** begin() const { return entries; }
    UnkBvhEntry** end() const { return entries + count; }

    // A shape placed by `xf` (fn_7100d5c370), swept by `delta` (fn_7100d5be04), a collider (fn_7100d5c1d0) or a
    // box (fn_7100d5c6dc) against the list's colliders on `mask`'s layers whose bounds overlap `bounds`.
    UnkFlags Query(UnkFlags flags, const UnkMtx44* xf, u32 arg2, UnkShape* shape, u32 mask, const UnkBox* bounds,
                   void* arg) asm("fn_7100d5c370");
    UnkFlags Query(UnkFlags flags, const UnkMtx44* xf, const Vec2* delta, UnkShape* shape, u32 mask,
                   const UnkBox* bounds, void* arg, bool flag) asm("fn_7100d5be04");
    UnkFlags Query(UnkFlags flags, UnkBvhCollider* collider, u32 arg2, u32 mask, const UnkBox* bounds, void* arg)
        asm("fn_7100d5c1d0");
    UnkFlags Query(UnkFlags flags, const UnkBox* box, u32 mask, const UnkBox* bounds, void* arg, u32 arg2)
        asm("fn_7100d5c6dc");
};
static_assert(sizeof(UnkSubTree) == 0x90, "UnkSubTree layout");

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
