// The collision world's unit (game::logic::collision, 0x7100d5761c-0x7100d5d23c): the queries of a
// collision object (a CCollider, or a CColliderGroup and the colliders it holds) against another
// object or a shape, and the 2D bounding-volume trees the world keeps its colliders in.
// Holds the dispatchers fn_7100d57d68, fn_7100d58ee4, fn_7100d58ab4, fn_7100d5761c and
// fn_7100d5823c (each tests a single collider directly, or each enabled collider of a group whose
// layer mask matches, and merges the results) and the tree query fn_7100d5b720 (a shape swept by an
// offset; its siblings are src/game_hi/fn_7100d5b484.cpp and the files next to it). A group's
// result is declared in the else branch so that it is built in the return slot (the original
// accumulates into it and reloads it after each call). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

// Assigns a 3D vector's x and y to a 2D one (the source type as src/game/CBatalloonAIComponent/fn_71003de7b4.cpp names it).
struct UnkVec3;
void fn_710007b444(void* dst, const UnkVec3* value);

// The result of a query (as in src/game/UnkCollisionContacts.cpp): bit 0 a hit, bit 1 stop.
// Returned in memory: its copy is user-written.
struct UnkColliderResult {
    u32 bits;

    UnkColliderResult() : bits(0) {}
    UnkColliderResult(u32 b) : bits(b) {}
    UnkColliderResult(const UnkColliderResult& o) : bits(o.bits) {}
};

inline const UnkColliderResult& NoColliderResult() {
    static UnkColliderResult s_none;
    return s_none;
}

static inline UnkColliderResult MakeHit(u32 bits) {
    UnkColliderResult h;
    h.bits = bits;
    return h;
}

// Both results' bits.
static inline UnkColliderResult Merge(const UnkColliderResult& a, const UnkColliderResult& b) {
    return MakeHit(a.bits | b.bits);
}

// A collider's placement: a row-major 3x4 transform, the translation in the last column.
struct UnkColliderXform {
    f32 m[3][4];
};

static inline CVector3D Translation(const UnkColliderXform* x) {
    CVector3D v;
    v.x = x->m[0][3];
    v.y = x->m[1][3];
    v.z = x->m[2][3];
    return v;
}

// A game::logic::collision::CCollider.
struct UnkCollider : UnkReflObject {
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
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual const UnkColliderXform* GetPrevXform();  // vf_0x80
    virtual const UnkColliderXform* GetXform();      // vf_0x88
    u8 unk_0x8[0x62];
    bool enabled;  // 0x6a
};

// A game::logic::collision::CColliderGroup: its colliders in an inline array (raw storage: the
// original compares begin() + count with begin() as a byte offset).
struct UnkColliderGroup : UnkReflObject {
    u8 unk_0x8[0x11c];
    u32 count;  // 0x124
    u8 colliders[8 * sizeof(UnkCollider*)];

    UnkCollider** begin() { return reinterpret_cast<UnkCollider**>(colliders); }
    UnkCollider** end() { return begin() + count; }
};

static inline bool IsCollider(UnkReflObject* obj) {
    return fn_710009d52c(obj->vf_0x0(), fn_7100d361c0());
}

static inline UnkCollider* AsCollider(UnkReflObject* obj) {
    return obj ? (fn_710009d50c(obj->vf_0x0(), fn_7100d361c0()) ? static_cast<UnkCollider*>(obj) : nullptr)
               : nullptr;
}

static inline UnkColliderGroup* AsGroup(UnkReflObject* obj) {
    return obj ? (fn_710009d50c(obj->vf_0x0(), fn_7100d454fc()) ? static_cast<UnkColliderGroup*>(obj) : nullptr)
               : nullptr;
}

UnkColliderResult fn_7100d57fd4(void* a, void* b, UnkCollider* collider, void* c, void* d, u32 e);

UnkColliderResult fn_7100d57d68(void* a, void* b, UnkReflObject* obj, void* c, u32 layers, void* d, u32 e) {
    if (IsCollider(obj)) {
        return fn_7100d57fd4(a, b, AsCollider(obj), c, d, e);
    } else {
        UnkColliderGroup* group = AsGroup(obj);
        UnkColliderResult result = NoColliderResult();
        for (UnkCollider** it = group->begin(); it != group->end(); ++it) {
            UnkCollider* collider = *it;
            if (collider->enabled && (collider->GetLayers() & layers))
                result = Merge(result, fn_7100d57fd4(a, b, collider, c, d, e));
        }
        return result;
    }
}

UnkColliderResult fn_7100d590ac(UnkCollider* collider, void* a, void* b, void* c, u32 d);

// A single collider is tested with 2 as the last argument whatever `d` is (as in the original).
UnkColliderResult fn_7100d58ee4(UnkReflObject* obj, void* a, void* b, u32 layers, void* c, u32 d) {
    if (IsCollider(obj)) {
        return fn_7100d590ac(AsCollider(obj), a, b, c, 2);
    } else {
        UnkColliderGroup* group = AsGroup(obj);
        UnkColliderResult result = NoColliderResult();
        for (UnkCollider** it = group->begin(); it != group->end(); ++it) {
            UnkCollider* collider = *it;
            if (collider->enabled && (collider->GetLayers() & layers))
                result = Merge(result, fn_7100d590ac(collider, a, b, c, d));
        }
        return result;
    }
}

UnkColliderResult fn_7100d595a4(void* a, void* b, UnkCollider* collider, void* c, void* d, void* e, bool flag);

UnkColliderResult fn_7100d58ab4(void* a, void* b, UnkReflObject* obj, void* c, void* d, u32 layers, void* e,
                                bool flag) {
    if (IsCollider(obj)) {
        UnkCollider* collider = AsCollider(obj);
        if (collider->enabled && (collider->GetLayers() & layers))
            return fn_7100d595a4(a, b, collider, c, d, e, flag);
        return NoColliderResult();
    } else {
        UnkColliderGroup* group = AsGroup(obj);
        UnkColliderResult result = NoColliderResult();
        for (UnkCollider** it = group->begin(); it != group->end(); ++it) {
            UnkCollider* collider = *it;
            if (collider->enabled && (collider->GetLayers() & layers))
                result = Merge(result, fn_7100d595a4(a, b, collider, c, d, e, flag));
        }
        return result;
    }
}

UnkColliderResult fn_7100d57ab4(UnkCollider* a, UnkCollider* b, void* c, u32 d);

UnkColliderResult fn_7100d5761c(UnkReflObject* objA, UnkReflObject* objB, u32 layersA, u32 layersB, void* c, u32 d) {
    if (IsCollider(objA)) {
        UnkCollider* a = AsCollider(objA);
        if (IsCollider(objB)) {
            UnkCollider* b = AsCollider(objB);
            if (b->enabled && (b->GetLayers() & layersB))
                return fn_7100d57ab4(a, b, c, d);
            return NoColliderResult();
        } else {
            UnkColliderGroup* groupB = AsGroup(objB);
            UnkColliderResult result = NoColliderResult();
            for (UnkCollider** it = groupB->begin(); it != groupB->end(); ++it) {
                UnkCollider* b = *it;
                if (b->enabled && (b->GetLayers() & layersB))
                    result = Merge(result, fn_7100d57ab4(a, b, c, d));
            }
            return result;
        }
    } else {
        UnkColliderGroup* groupA = AsGroup(objA);
        UnkColliderResult result = NoColliderResult();
        for (UnkCollider** itA = groupA->begin(); itA != groupA->end(); ++itA) {
            UnkCollider* a = *itA;
            if (!a->enabled || !(a->GetLayers() & layersA))
                continue;
            if (IsCollider(objB)) {
                UnkCollider* b = AsCollider(objB);
                if (b->enabled && (b->GetLayers() & layersB))
                    result = Merge(result, fn_7100d57ab4(a, b, c, d));
            } else {
                UnkColliderGroup* groupB = AsGroup(objB);
                for (UnkCollider** itB = groupB->begin(); itB != groupB->end(); ++itB) {
                    UnkCollider* b = *itB;
                    if (b->enabled && (b->GetLayers() & layersB))
                        result = Merge(result, fn_7100d57ab4(a, b, c, d));
                }
            }
        }
        return result;
    }
}

UnkColliderResult fn_7100d5925c(UnkCollider* a, UnkCollider* b, const CVector2D* delta, void* c);

// The move of `a` since its previous placement.
static inline CVector3D OwnMotion(UnkCollider* a) {
    CVector3D to = Translation(a->GetXform());
    CVector3D from = Translation(a->GetPrevXform());
    CVector3D move;
    move.x = to.x - from.x;
    move.y = to.y - from.y;
    move.z = to.z - from.z;
    return move;
}

// The offset `a` sweeps by: `delta`, or with `ownMotion` the move of `a` (its x and y).
static inline void SweepOffset(CVector2D* d, const CVector2D* delta, UnkCollider* a, bool ownMotion) {
    d->x = delta->x;
    d->y = delta->y;
    if (ownMotion) {
        CVector3D move = OwnMotion(a);
        fn_710007b444(d, reinterpret_cast<const UnkVec3*>(&move));
    }
}

// The sweep of `objA` by `delta` against `objB`. Two single colliders are tested with `delta` as it
// is: `ownMotion` only applies when either side is a group (as in the original).
UnkColliderResult fn_7100d5823c(UnkReflObject* objA, UnkReflObject* objB, const CVector2D* delta, u32 layersA,
                                u32 layersB, void* c, bool ownMotion) {
    if (IsCollider(objA)) {
        UnkCollider* a = AsCollider(objA);
        if (IsCollider(objB)) {
            UnkCollider* b = AsCollider(objB);
            if (b->enabled && (b->GetLayers() & layersB))
                return fn_7100d5925c(a, b, delta, c);
            return NoColliderResult();
        } else {
            UnkColliderGroup* groupB = AsGroup(objB);
            UnkColliderResult result = NoColliderResult();
            for (UnkCollider** it = groupB->begin(); it != groupB->end(); ++it) {
                UnkCollider* b = *it;
                if (b->enabled && (b->GetLayers() & layersB)) {
                    CVector2D d;
                    SweepOffset(&d, delta, a, ownMotion);
                    result = Merge(result, fn_7100d5925c(a, b, &d, c));
                }
            }
            return result;
        }
    } else {
        UnkColliderGroup* groupA = AsGroup(objA);
        UnkColliderResult result = NoColliderResult();
        for (UnkCollider** itA = groupA->begin(); itA != groupA->end(); ++itA) {
            UnkCollider* a = *itA;
            if (!a->enabled || !(a->GetLayers() & layersA))
                continue;
            if (IsCollider(objB)) {
                UnkCollider* b = AsCollider(objB);
                if (b->enabled && (b->GetLayers() & layersB)) {
                    CVector2D d;
                    SweepOffset(&d, delta, a, ownMotion);
                    result = Merge(result, fn_7100d5925c(a, b, &d, c));
                }
            } else {
                UnkColliderGroup* groupB = AsGroup(objB);
                for (UnkCollider** itB = groupB->begin(); itB != groupB->end(); ++itB) {
                    UnkCollider* b = *itB;
                    if (b->enabled && (b->GetLayers() & layersB)) {
                        CVector2D d;
                        SweepOffset(&d, delta, a, ownMotion);
                        result = Merge(result, fn_7100d5925c(a, b, &d, c));
                    }
                }
            }
        }
        return result;
    }
}

// A shape placed in a tree query (as in src/game_hi/fn_7100d5bfbc.cpp): its cached 2D bounds,
// refreshed through vf_0x98 when marked dirty.
struct UnkBvhBox {
    f32 minX, minY, maxX, maxY;
};

struct UnkBvhShape {
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
    virtual void RefreshBounds();  // vf_0x98
    u8 unk_0x8[0x38];
    UnkBvhBox bounds;  // 0x40
    u8 unk_0x50[0x10];
    bool boundsDirty;  // 0x60
};

// A tree node: its box, then the index of the next node outside its subtree (>= 0) or a leaf (< 0).
struct UnkBvhNode {
    f32 minX, minY, maxX, maxY;
    s32 index;
};

// What the leaf visitor of a sweep query receives.
struct UnkBvhSweepQuery {
    u32 mask;
    void* arg;
    void* field_0x10;
    UnkColliderResult result;
    UnkBvhShape* shape;
    CVector2D delta;
};

// One of a tree's sub-trees (0x90 bytes), queried when it holds a layer the tree does not.
struct UnkBvhSubTree {
    u8 unk_0x0[8];
    u32 field_0x8;
    u32 field_0xc;
    u8 unk_0x10[0x80];
};

// The sweep query of one sub-tree.
UnkColliderResult fn_7100d5b918(UnkBvhSubTree* self, UnkColliderResult result, UnkBvhShape* shape,
                                const CVector2D* delta, u32 mask, const UnkBvhBox* bounds, void* arg);

// The collision world's tree (layout as in src/game_hi/fn_7100d5b484.cpp).
struct UnkBvhTree {
    u8 unk_0x0[0x5c];
    u32 field_0x5c;  // the layers it holds
    UnkBvhNode* nodes;
    u32 nodeCount;
    u8 unk_0x6c[0x24];
    u8 field_0x90[0x28];
    u32 subTreeCount;
    u8 subTrees[4 * sizeof(UnkBvhSubTree)];

    UnkBvhSubTree* SubBegin() { return reinterpret_cast<UnkBvhSubTree*>(subTrees); }
    UnkBvhSubTree* SubEnd() { return SubBegin() + subTreeCount; }
};

bool fn_7100d5cc80(UnkBvhSweepQuery* query, const UnkBvhNode* node);

// Whether anything on one of `layers` overlaps `shape` swept by `delta` (the union of its bounds
// before and after the move): the tree's leaves first, then the first sub-tree holding the layers.
// NON_MATCHING(minor): the two load pairs after the refresh are scheduled the other way round (the
// offset's ldp before the bounds' minimum ldp); no store or call lies between them.
bool fn_7100d5b720(UnkBvhTree* self, u32 layers, UnkBvhShape* shape, const CVector2D* delta, void* arg) {
    if (shape->boundsDirty)
        shape->RefreshBounds();
    f32 dx = delta->x, dy = delta->y;
    f32 x0 = shape->bounds.minX, y0 = shape->bounds.minY;
    f32 x1 = shape->bounds.maxX, y1 = shape->bounds.maxY;
    UnkBvhBox bounds;
    f32 mx0 = x0 + dx, my0 = y0 + dy, mx1 = x1 + dx, my1 = y1 + dy;
    bounds.minX = mx0 < x0 ? mx0 : x0;
    bounds.minY = my0 < y0 ? my0 : y0;
    bounds.maxX = mx1 > x1 ? mx1 : x1;
    bounds.maxY = my1 > y1 ? my1 : y1;
    CVector2D d;
    d.x = dx;
    d.y = dy;
    UnkColliderResult result = 0;
    if (self->field_0x5c & layers) {
        UnkBvhSweepQuery query = {layers, arg, self->field_0x90, result, shape, d};
        u32 count = self->nodeCount;
        u32 i = 0;
        while (i < count) {
            const UnkBvhNode& n = self->nodes[i];
            if (bounds.maxX >= n.minX && bounds.minX <= n.maxX && bounds.maxY >= n.minY && bounds.minY <= n.maxY) {
                if (n.index < 0 && !fn_7100d5cc80(&query, &n))
                    break;
                ++i;
            } else {
                i = n.index >= 0 ? (u32)n.index : i + 1;
            }
        }
        result = query.result;
    }
    if (!(result.bits & 2)) {
        for (UnkBvhSubTree* t = self->SubBegin(); t != self->SubEnd(); ++t) {
            if ((t->field_0x8 & layers) && !(t->field_0xc & layers)) {
                result.bits |= fn_7100d5b918(t, result, shape, delta, layers, &bounds, arg).bits;
                break;
            }
        }
    }
    return result.bits & 1;
}
