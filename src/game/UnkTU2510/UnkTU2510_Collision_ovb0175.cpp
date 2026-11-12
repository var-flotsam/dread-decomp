// The collision world's unit (game::logic::collision, 0x7100d5761c-0x7100d5d23c; unit 2510 of tools/tu_map.py,
// no reliable name): the node visitors of the 2D bounding-volume tree (one per query kind: a box, a shape, a
// swept shape, a collider against a collider and a swept collider; each skips a leaf whose collider is
// disabled or whose layers miss the query mask, runs the dispatcher on the others and reports whether the query
// goes on), the box, shape-sweep and set queries over a set of colliders, and the leaf tests of the dispatchers
// (UnkCollisionWorld.cpp) that bound-check a collider and hand it to the shape tester. Names and layouts are
// placeholders.
#include <new>

#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

// The result of a query (as in UnkCollisionWorld.cpp): bit 0 a hit, bit 1 stop. Returned in memory: its copy is
// user-written. Also the result of the shape tester (fn_7100d51368).
struct UnkShapeTestResult_ovb0107 {
    union {
        u32 flags;
        u32 bits;
    };
    u32 field_0x4;
    UnkShapeTestResult_ovb0107() : bits(0) {}
    UnkShapeTestResult_ovb0107(u32 b) : bits(b) {}
    UnkShapeTestResult_ovb0107(const UnkShapeTestResult_ovb0107& o) : bits(o.bits) {}
};
using UnkColliderResult = UnkShapeTestResult_ovb0107;

// The "skip" result and its guard (shared with the other visitors).
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline void InitSkip() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
}

// The running result of a visit (bit 0 a hit, bit 1 stop).
struct UnkVisitBits_b0175 {
    u32 bits;
};

struct UnkLeafCollider_b0175 {
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
    virtual bool IsSame(void* other);  // vf_0x68
    u8 unk_0x8[0x62];
    bool enabled;  // 0x6a
};

struct UnkLeaf_b0175 {
    u8 unk_0x0[0x10];
    UnkLeafCollider_b0175* collider;
};
struct UnkLeafTable_b0175 {
    UnkLeaf_b0175** entries;
};
static inline UnkLeafCollider_b0175* LeafCollider(UnkLeafTable_b0175* table, s32 index) {
    UnkLeaf_b0175* leaf = table->entries[index < 0 ? (index & 0x7ffffff) : 0xfffffff];
    return leaf ? leaf->collider : nullptr;
}

struct UnkQueryCtx;
struct UnkNode {
    f32 minX, minY, maxX, maxY;
    s32 index;
};

// ---- box query
struct UnkBoxCtx_b0175 {
    u32 mask;
    void* arg;
    UnkLeafTable_b0175* table;
    UnkVisitBits_b0175 result;
    void* box;
    void* bounds;
};
UnkColliderResult fn_7100d58ee4(UnkReflObject* obj, void* a, void* b, u32 layers, void* c, u32 d);

bool fn_7100d5d0bc(UnkQueryCtx* c, const UnkNode* node) {
    UnkBoxCtx_b0175* ctx = reinterpret_cast<UnkBoxCtx_b0175*>(c);
    UnkLeafCollider_b0175* collider = LeafCollider(ctx->table, node->index);
    if (collider && collider->enabled && (collider->GetLayers() & ctx->mask)) {
        InitSkip();
        ctx->result.bits |= fn_7100d58ee4(reinterpret_cast<UnkReflObject*>(collider), ctx->box, ctx->bounds,
                                          ctx->mask, ctx->arg, 2).bits;
    }
    return !(ctx->result.bits & 2);
}

// ---- shape query
struct UnkShapeCtx_b0175 {
    u32 mask;
    void* arg;
    UnkLeafTable_b0175* table;
    UnkVisitBits_b0175 result;
    void* shape;
    u8 transform[0x40];
    void* bounds;  // 0x68
    u16 arg2;      // 0x70
};
UnkColliderResult fn_7100d57d68(void* a, void* b, UnkReflObject* obj, void* c, u32 layers, void* d, u32 e);

bool fn_7100d5cfb4(UnkQueryCtx* c, const UnkNode* node) {
    UnkShapeCtx_b0175* ctx = reinterpret_cast<UnkShapeCtx_b0175*>(c);
    UnkLeafCollider_b0175* collider = LeafCollider(ctx->table, node->index);
    if (collider && collider->enabled && (collider->GetLayers() & ctx->mask) && !collider->IsSame(ctx->shape)) {
        InitSkip();
        ctx->result.bits |= fn_7100d57d68(ctx->shape, ctx->transform, reinterpret_cast<UnkReflObject*>(collider),
                                          ctx->bounds, ctx->mask, ctx->arg, ctx->arg2).bits;
    }
    return !(ctx->result.bits & 2);
}

// ---- shape sweep query
struct UnkSweepCtx_b0175 {
    u32 mask;
    void* arg;
    UnkLeafTable_b0175* table;
    UnkVisitBits_b0175 result;
    void* shape;
    u8 transform[0x40];
    u8 delta[8];   // 0x68
    void* field_0x70;
};
UnkColliderResult fn_7100d58ab4(void* a, void* b, UnkReflObject* obj, void* c, void* d, u32 layers, void* e,
                                bool flag);

bool fn_7100d5cd9c(UnkQueryCtx* c, const UnkNode* node) {
    UnkSweepCtx_b0175* ctx = reinterpret_cast<UnkSweepCtx_b0175*>(c);
    UnkLeafCollider_b0175* collider = LeafCollider(ctx->table, node->index);
    if (collider && collider->enabled && (collider->GetLayers() & ctx->mask) && !collider->IsSame(ctx->shape)) {
        InitSkip();
        ctx->result.bits |= fn_7100d58ab4(ctx->shape, ctx->transform, reinterpret_cast<UnkReflObject*>(collider),
                                          ctx->delta, ctx->field_0x70, ctx->mask, ctx->arg, false).bits;
    }
    return !(ctx->result.bits & 2);
}

// ---- collider vs collider
struct UnkPairCtx_b0175 {
    u32 mask;
    void* arg;
    UnkLeafTable_b0175* table;
    UnkVisitBits_b0175 result;
    UnkLeafCollider_b0175* self;
    u16 arg2;  // 0x28
};
UnkColliderResult fn_7100d5761c(UnkReflObject* objA, UnkReflObject* objB, u32 layersA, u32 layersB, void* c, u32 d);

bool fn_7100d5cea8(UnkQueryCtx* c, const UnkNode* node) {
    UnkPairCtx_b0175* ctx = reinterpret_cast<UnkPairCtx_b0175*>(c);
    UnkLeafCollider_b0175* collider = LeafCollider(ctx->table, node->index);
    if (collider && collider->enabled && (collider->GetLayers() & ctx->mask) && ctx->self != collider) {
        InitSkip();
        UnkLeafCollider_b0175* self = ctx->self;
        ctx->result.bits |= fn_7100d5761c(reinterpret_cast<UnkReflObject*>(self),
                                          reinterpret_cast<UnkReflObject*>(collider), self->GetLayers(), ctx->mask,
                                          ctx->arg, ctx->arg2).bits;
    }
    return !(ctx->result.bits & 2);
}

struct UnkPairSweepCtx_b0175 {
    u32 mask;
    void* arg;
    UnkLeafTable_b0175* table;
    UnkVisitBits_b0175 result;
    UnkLeafCollider_b0175* self;
    u8 delta[8];  // 0x28
};
UnkColliderResult fn_7100d5823c(UnkReflObject* objA, UnkReflObject* objB, const void* delta, u32 layersA,
                                u32 layersB, void* c, bool ownMotion);

bool fn_7100d5cc80(UnkQueryCtx* c, const UnkNode* node) {
    UnkPairSweepCtx_b0175* ctx = reinterpret_cast<UnkPairSweepCtx_b0175*>(c);
    UnkLeafCollider_b0175* collider = LeafCollider(ctx->table, node->index);
    if (collider && collider->enabled && (collider->GetLayers() & ctx->mask) && ctx->self != collider) {
        InitSkip();
        UnkLeafCollider_b0175* self = ctx->self;
        ctx->result.bits |= fn_7100d5823c(reinterpret_cast<UnkReflObject*>(self),
                                          reinterpret_cast<UnkReflObject*>(collider), ctx->delta, self->GetLayers(),
                                          ctx->mask, ctx->arg, false).bits;
    }
    return !(ctx->result.bits & 2);
}

struct UnkLayered_58e24 {
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
};

struct UnkBox_58e24 {
    f32 minX, minY, maxX, maxY;
};

bool fn_7100d58e24(UnkLayered_58e24* obj, const UnkBox_58e24* box, void* arg, u32 flags) {
    if (!obj)
        return false;
    const f32 big = 3.4028235e+38f;
    f32 x0 = box->minX, y0 = box->minY;
    f32 loX = !(x0 >= big) ? x0 : big;
    f32 loY = !(y0 >= big) ? y0 : big;
    f32 hiX = !(x0 < -big) ? x0 : -big;
    f32 hiY = !(y0 < -big) ? y0 : -big;
    f32 x1 = box->maxX, y1 = box->maxY;
    UnkBox_58e24 bounds;
    bounds.minX = loX < x1 ? loX : x1;
    bounds.minY = loY < y1 ? loY : y1;
    bounds.maxX = hiX > x1 ? hiX : x1;
    bounds.maxY = hiY > y1 ? hiY : y1;
    return fn_7100d58ee4(reinterpret_cast<UnkReflObject*>(obj), const_cast<UnkBox_58e24*>(box), &bounds,
                         obj->GetLayers(), arg, flags).bits & 1;
}

struct UnkLayered_58cfc {
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
    u8 unk_0x8[0x62];
    bool enabled;  // 0x6a
};

struct UnkBox_58cfc {
    f32 minX, minY, maxX, maxY;
};

struct UnkSet_58cfc {
    UnkLayered_58cfc** slots;
    u32 count;
};

bool fn_7100d58cfc(UnkSet_58cfc* self, const UnkBox_58cfc* box, void* arg, u32 flags) {
    bool hit = false;
    UnkLayered_58cfc** end = self->slots + self->count;
    for (UnkLayered_58cfc** it = self->slots; it != end; ++it) {
        UnkLayered_58cfc* obj = *it;
        if (!obj || !obj->enabled)
            continue;
        const f32 big = 3.4028235e+38f;
        f32 x0 = box->minX, y0 = box->minY;
        f32 loX = !(x0 >= big) ? x0 : big;
        f32 loY = !(y0 >= big) ? y0 : big;
        f32 hiX = !(x0 < -big) ? x0 : -big;
        f32 hiY = !(y0 < -big) ? y0 : -big;
        f32 x1 = box->maxX, y1 = box->maxY;
        UnkBox_58cfc bounds;
        bounds.minX = loX < x1 ? loX : x1;
        bounds.minY = loY < y1 ? loY : y1;
        bounds.maxX = hiX > x1 ? hiX : x1;
        bounds.maxY = hiY > y1 ? hiY : y1;
        UnkColliderResult r = fn_7100d58ee4(reinterpret_cast<UnkReflObject*>(obj), const_cast<UnkBox_58cfc*>(box), &bounds,
                                            obj->GetLayers(), arg, flags);
        if (r.bits & 1)
            hit = true;
    }
    return hit;
}

struct UnkAABB3_58958 {
    CVector3D min, max;
};

struct UnkXform_58958;
struct UnkShape_58958 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkAABB3_58958 GetBounds(const UnkXform_58958* xf);  // vf_0x28
};

struct UnkEntry_58958 {
    u8 unk_0x0[0x6a];
    bool enabled;  // 0x6a
};

struct UnkSet_58958 {
    UnkEntry_58958** slots;
    u32 count;
};

struct UnkBox_58958 {
    f32 minX, minY, maxX, maxY;
};
struct UnkBounds2_58958 {
    CVector2D lo, hi;
    UnkBounds2_58958(const UnkAABB3_58958& b) {
        fn_710007b430(&lo, &b.min);
        fn_710007b430(&hi, &b.max);
    }
};

bool fn_7100d58958(UnkShape_58958* shape, const UnkXform_58958* xf, UnkSet_58958* set, const CVector2D* delta,
                   u32 mask, void* arg) {
    bool hit = false;
    UnkEntry_58958** end = set->slots + set->count;
    for (UnkEntry_58958** it = set->slots; it != end; ++it) {
        UnkEntry_58958* entry = *it;
        if (!entry || !entry->enabled)
            continue;
        UnkBounds2_58958 cur(shape->GetBounds(xf));
        f32 lx = cur.lo.x, ly = cur.lo.y, hx = cur.hi.x, hy = cur.hi.y;
        UnkBox_58958 box;
        f32 dx = delta->x, dy = delta->y;
        f32 loX = lx + dx, loY = ly + dy;
        f32 hiX = hx + dx, hiY = hy + dy;
        box.minX = loX < lx ? loX : lx;
        box.minY = loY < ly ? loY : ly;
        box.maxX = hiX > hx ? hiX : hx;
        box.maxY = hiY > hy ? hiY : hy;
        UnkColliderResult r = fn_7100d58ab4(shape, const_cast<UnkXform_58958*>(xf), reinterpret_cast<UnkReflObject*>(entry),
                                            const_cast<CVector2D*>(delta), &box, mask, arg, false);
        if (r.bits & 1)
            hit = true;
    }
    return hit;
}

struct UnkAABB3_57ca0 {
    CVector3D min, max;
};
struct UnkXform_57ca0;
struct UnkLavaDropsShape_A2_b71;

struct UnkShape_57ca0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkAABB3_57ca0 GetBounds(const UnkXform_57ca0* xf);  // vf_0x28
};
struct UnkLayered_57ca0 {
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
};
struct UnkBounds2_57ca0 {
    CVector2D lo, hi;
    UnkBounds2_57ca0(const UnkAABB3_57ca0& b) {
        fn_710007b430(&lo, &b.min);
        fn_710007b430(&hi, &b.max);
    }
};

bool fn_7100d57ca0(UnkLavaDropsShape_A2_b71* self_, void* xf_, void* other_, void* out, u32 flags) {
    UnkShape_57ca0* self = reinterpret_cast<UnkShape_57ca0*>(self_);
    UnkLayered_57ca0* other = static_cast<UnkLayered_57ca0*>(other_);
    if (!self || !other)
        return false;
    UnkBounds2_57ca0 box(self->GetBounds(static_cast<UnkXform_57ca0*>(xf_)));
    return fn_7100d57d68(self, xf_, reinterpret_cast<UnkReflObject*>(other), &box, other->GetLayers(), out, flags).bits & 1;
}

struct UnkShape;

struct UnkShapeTester_A5_b68;

static inline UnkShapeTester_A5_b68* Tester() {
    UnkShapeTester_A5_b68* t = g_unk_71726a47a0;
    if (!t) {
        t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return t;
}

// A collider (game::logic::collision::CCollider) as the tests use it.
struct UnkCollider {
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
    virtual void UpdateBounds();  // vf_0x98
    u8 unk_0x8[0x38];
    f32 minX;  // 0x40
    f32 minY;  // 0x44
    f32 maxX;  // 0x48
    f32 maxY;  // 0x4c
    u8 unk_0x50[0x10];
    bool dirty;  // 0x60
    u8 unk_0x61[0xf7];
    void* owner;  // 0x158
};

extern void* g_unk_7101aa8898[];

// ---- fn_7100d590ac
struct UnkShapeFilter_590ac {
    void** vtable;
    UnkCollider* node;
    u64 field_0x10;
    void* filter;
    bool field_0x20;
    UnkShapeFilter_590ac(UnkCollider* n, void* f)
        : vtable(g_unk_7101aa8898 + 2), node(n), field_0x10(0), filter(f), field_0x20(false) {}
};
UnkColliderResult fn_7100d56604(UnkShapeTester_A5_b68* tester, void* a, void* owner, const UnkMatrix44& transform,
                                UnkShapeFilter_590ac* filter, u32 flags);

UnkColliderResult fn_7100d590ac(UnkCollider* collider, void* a, void* b, void* c, u32 d) {
    if (collider->dirty)
        collider->UpdateBounds();
    f32* box = static_cast<f32*>(b);
    if (!(collider->maxX >= box[0] && collider->minX <= box[2] && collider->maxY >= box[1] &&
          collider->minY <= box[3])) {
        InitSkip();
        return UnkColliderResult(g_unk_71726a4778.value);
    }
    if (c) {
        UnkShapeFilter_590ac filter(collider, c);
        UnkShapeTester_A5_b68* tester = Tester();
        return fn_7100d56604(tester, a, collider->owner, fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider)),
                             &filter, d);
    }
    UnkShapeTester_A5_b68* tester = Tester();
    return fn_7100d56604(tester, a, collider->owner, fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider)), nullptr,
                         d);
}

// ---- fn_7100d57ab4
struct UnkShapeFilter_ovb0107 {
    void** vtable;
    UnkCollider* node;
    UnkCollider* field_0x10;
    void* filter;
    bool field_0x20;
    UnkShapeFilter_ovb0107(UnkCollider* n, UnkCollider* other, void* f)
        : vtable(g_unk_7101aa8898 + 2), node(n), field_0x10(other), filter(f), field_0x20(false) {}
};

UnkColliderResult fn_7100d57ab4(UnkCollider* a, UnkCollider* b, void* c, u32 d) {
    if (b->dirty)
        b->UpdateBounds();
    if (a->dirty)
        a->UpdateBounds();
    if (!(a->maxX >= b->minX && a->minX <= b->maxX && a->maxY >= b->minY && a->minY <= b->maxY)) {
        InitSkip();
        return UnkColliderResult(g_unk_71726a4778.value);
    }
    if (c) {
        UnkShapeFilter_ovb0107 filter(b, a, c);
        UnkShapeTester_A5_b68* tester = Tester();
        void* ownerA = a->owner;
        UnkMatrix44 matA = fn_7100d36a2c(reinterpret_cast<UnkShape*>(a));
        void* ownerB = b->owner;
        return fn_7100d51368(tester, ownerA, &matA, ownerB, fn_7100d36a2c(reinterpret_cast<UnkShape*>(b)), &filter, d);
    }
    UnkShapeTester_A5_b68* tester = Tester();
    void* ownerA = a->owner;
    UnkMatrix44 matA = fn_7100d36a2c(reinterpret_cast<UnkShape*>(a));
    void* ownerB = b->owner;
    return fn_7100d51368(tester, ownerA, &matA, ownerB, fn_7100d36a2c(reinterpret_cast<UnkShape*>(b)), nullptr, d);
}


template <u32 Offset>
struct UnkWeakRefAt;
void fn_710092ef94(UnkWeakRefAt<0>* self, UnkWeakTarget* other);
void fn_710092f0b4(UnkWeakRefAt<8>* self, UnkWeakTarget* other);

struct UnkHitRef_597b4 {
    UnkRefObject* ptr;
    UnkHitRef_597b4() : ptr(nullptr) {}
    ~UnkHitRef_597b4() {
        if (UnkRefObject* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkHit_597b4 {
    UnkHitRef_597b4 object;
    UnkHitRef_597b4 other;
    s32 id;
    u32 values[12];
    UnkHit_597b4() : id(-1), values() {}
};

struct UnkQueryIn_597b4 {
    s32 id;
    u32 values[10];
    u32 flags;  // 0x2c
};

struct UnkQueryTarget_597b4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkColliderResult Query(UnkHit_597b4* hit);  // vf_0x10
};

struct UnkCollider_597b4 {
    u8 unk_0x0[0x158];
    struct Owner {
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
        virtual u32 vf_0x90(s32 id);
    }* owner;
};

struct UnkSelf_597b4 {
    u8 unk_0x0[8];
    UnkReflObject* target;      // 0x8
    UnkWeakTarget* second;      // 0x10
    UnkQueryTarget_597b4* query;  // 0x18
    bool hit;                   // 0x20
};

UnkColliderResult fn_7100d597b4(UnkSelf_597b4* self, const UnkQueryIn_597b4* q) {
    UnkHit_597b4 desc;
    fn_710092ef94(reinterpret_cast<UnkWeakRefAt<0>*>(&desc), reinterpret_cast<UnkWeakTarget*>(self->target));
    desc.id = q->id;
    desc.values[0] = q->values[0];
    desc.values[1] = q->values[1];
    desc.values[2] = q->values[2];
    desc.values[3] = q->values[3];
    desc.values[4] = q->values[4];
    desc.values[5] = q->values[5];
    desc.values[6] = q->values[6];
    desc.values[7] = q->values[7];
    desc.values[8] = q->values[8];
    desc.values[9] = q->values[9];
    desc.values[10] = q->flags;
    fn_710092f0b4(reinterpret_cast<UnkWeakRefAt<8>*>(&desc), self->second);
    u32 r;
    if (fn_710009d52c(self->target->vf_0x0(), fn_7100d361c0())) {
        UnkReflObject* obj = self->target;
        UnkCollider_597b4* collider =
            fn_710009d50c(obj->vf_0x0(), fn_7100d361c0()) ? reinterpret_cast<UnkCollider_597b4*>(obj) : nullptr;
        auto* owner = collider->owner;
        r = owner ? ((q->flags & 1) ? owner->vf_0x90(q->id) : 0) : 0;
    } else {
        r = 0;
    }
    desc.values[10] = desc.values[10] | 0x40;
    desc.values[11] = r;
    UnkColliderResult out = self->query->Query(&desc);
    UnkColliderResult result(out);
    self->hit = self->hit || (out.bits & 1);
    return result;
}

// Appends a sub-tree for a layer mask: an empty id (the string pool is created first when it doesn't exist yet), the lists
// the sub-tree keeps, then the id and the mask (and its complement) set.
// An element of the tree's sub-tree list (0x90 bytes): a string id and its layer mask, followed by
// the lists the sub-tree keeps.
struct UnkTreeSubTail_c96c {
    u32 mask;                       // 0x8
    u32 notMask;                    // 0xc
    base::UnkVector<void*> list;    // 0x10
    bool field_0x38;
    u32 field_0x3c;
    base::UnkVector<void*> boxes;   // 0x40
    base::UnkVector<void*> extras;  // 0x68

    UnkTreeSubTail_c96c() : mask(0), notMask(~0u), list(7), field_0x38(false), field_0x3c(0), boxes(0xf), extras(0xf) {}
};
static_assert(sizeof(UnkTreeSubTail_c96c) == 0x88, "UnkTreeSubTail_c96c layout");

struct UnkTreeSub_c96c {
    UnkStringData* id;  // 0x0, a CStrId
    UnkTreeSubTail_c96c tail;
};

#pragma pack(push, 4)
struct UnkTree_c96c {
    u8 unk_0x0[0xb8];
    u32 subCount;  // 0xb8
    u8 subs[4 * sizeof(UnkTreeSub_c96c)];  // 0xbc
};
#pragma pack(pop)

__attribute__((flatten)) void fn_7100d5c96c(UnkTree_c96c* self, const CStrId* id, u32 mask) {
    UnkTreeSub_c96c* base = reinterpret_cast<UnkTreeSub_c96c*>(self->subs);
    u32 n = self->subCount;
    UnkTreeSub_c96c* sub = base + n;
    sub->id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    sub->id = &g_unk_7101d47108->empty;
    UnkTreeSubTail_c96c* tail = new (&base[n].tail) UnkTreeSubTail_c96c();
    ++self->subCount;
    *reinterpret_cast<CStrId*>(&sub->id) = *id;
    tail->mask = mask;
    tail->notMask = ~mask;
}
