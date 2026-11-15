// Unit 2510 (collision world): a shape swept by a 2D offset against every collider of a plain list (no
// tree), keeping those whose layers and bounds overlap it and adding their sweep results.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector2D;
using base::reflection::UnkReflObject;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

// The result of a query (as in src/game/UnkCollisionContacts.cpp): bit 0 a hit, bit 1 stop.
// Returned in memory: its copy is user-written.
struct UnkColliderResult {
    u32 bits;

    UnkColliderResult() : bits(0) {}
    UnkColliderResult(u32 b) : bits(b) {}
    UnkColliderResult(const UnkColliderResult& o) : bits(o.bits) {}
};

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

UnkColliderResult fn_7100d5823c(UnkReflObject* objA, UnkReflObject* objB, const void* delta, u32 layersA,
                                u32 layersB, void* c, bool ownMotion);

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

struct UnkCollider_d5c1d0 {
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
    virtual void UpdateBounds();  // vf_0x98
    u8 pad_0x8[0x38];
    f32 minX, minY, maxX, maxY;  // 0x40
    u8 pad_0x50[0x10];
    bool boundsDirty;  // 0x60
    u8 pad_0x61[0x9];
    bool enabled;  // 0x6a
};

struct UnkEntry_d5c1d0 {
    u8 pad_0x0[0x10];
    UnkCollider_d5c1d0* collider;
};

struct UnkList_d5c1d0 {
    u8 pad_0x0[0x10];
    UnkEntry_d5c1d0** entries;
    u32 count;

    UnkEntry_d5c1d0** begin() const { return entries; }
    UnkEntry_d5c1d0** end() const { return entries + count; }
};

static inline bool Overlaps(const UnkCollider_d5c1d0* c, const UnkBvhBox* box) {
    return c->maxX >= box->minX && c->minX <= box->maxX && c->maxY >= box->minY && c->minY <= box->maxY;
}

struct UnkBvhSubTree;  // src/game/logic/collision/UnkCollisionWorld.cpp; its bytes from 0x10 are this list

// The colliders of the list, one by one: those on the mask's layers whose bounds overlap `bounds` are swept
// against `shape` moved by `delta`. Stops early once a test asks to.
UnkColliderResult fn_7100d5b918(UnkBvhSubTree* tree, UnkColliderResult result, UnkBvhShape* shape,
                                const CVector2D* delta, u32 mask, const UnkBvhBox* bounds, void* arg) {
    UnkList_d5c1d0* self = reinterpret_cast<UnkList_d5c1d0*>(tree);
    UnkColliderResult total(result);
    UnkCollider_d5c1d0* collider = reinterpret_cast<UnkCollider_d5c1d0*>(shape);
    UnkEntry_d5c1d0** end = self->end();
    for (UnkEntry_d5c1d0** it = self->begin(); !(total.bits & 2) && it != end; ++it) {
        UnkEntry_d5c1d0* entry = *it;
        UnkCollider_d5c1d0* other = entry ? entry->collider : nullptr;
        if (!other || !other->enabled)
            continue;
        if (!(other->GetLayers() & mask) || other == collider)
            continue;
        if (other->boundsDirty)
            other->UpdateBounds();
        if (!Overlaps(other, bounds))
            continue;
        InitSkip();
        total.bits |= fn_7100d5823c(reinterpret_cast<UnkReflObject*>(collider), reinterpret_cast<UnkReflObject*>(other),
                                    delta, collider->GetLayers(), mask, arg, false).bits;
    }
    return total;
}
