// Unit 2510 (collision world): a shape placed by a transform against every collider of a plain list (no
// tree), keeping those on the mask's layers whose bounds overlap it and adding their test results.
// Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "game/UnkTU365.h"
#include "types.h"

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

UnkColliderResult fn_7100d57d68(void* a, void* b, UnkReflObject* obj, void* c, u32 layers, void* d, u32 e);

struct UnkFlags {
    u32 bits;
    UnkFlags() : bits(0) {}
    UnkFlags(const UnkFlags& o) : bits(o.bits) {}
};

struct Vec3 {
    f32 x, y, z;
};

struct Vec2 {
    f32 x, y;
    Vec2(const Vec3& v);  // fn_710007b430
};

struct UnkAABB3 {
    Vec3 min, max;
};

struct UnkBox {
    Vec2 min, max;
    UnkBox(const UnkAABB3& b) : min(b.min), max(b.max) {}
};

struct UnkMtx44;
struct UnkShape;

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

static inline bool Overlaps(const UnkCollider_d5c1d0* c, const UnkBox* box) {
    return c->maxX >= box->min.x && c->minX <= box->max.x && c->maxY >= box->min.y && c->minY <= box->max.y;
}

// The colliders of the list, one by one: those on the mask's layers (other than the shape itself) whose bounds
// overlap `bounds` are tested against `shape` placed by `xf`. Stops early once a test asks to.
UnkFlags fn_7100d5c370(UnkList_d5c1d0* self, UnkFlags flags, const UnkMtx44* xf, u32 arg2, UnkShape* shape, u32 mask,
                       const UnkBox* bounds, void* arg) {
    UnkFlags result(flags);
    UnkEntry_d5c1d0** end = self->end();
    for (UnkEntry_d5c1d0** it = self->begin(); !(result.bits & 2) && it != end; ++it) {
        UnkEntry_d5c1d0* entry = *it;
        UnkCollider_d5c1d0* other = entry ? entry->collider : nullptr;
        if (!other || !other->enabled)
            continue;
        if (!(other->GetLayers() & mask) || other->IsSame(shape))
            continue;
        if (other->boundsDirty)
            other->UpdateBounds();
        if (!Overlaps(other, bounds))
            continue;
        InitSkip();
        result.bits |= fn_7100d57d68(shape, const_cast<UnkMtx44*>(xf), reinterpret_cast<UnkReflObject*>(other),
                                     const_cast<UnkBox*>(bounds), mask, arg, arg2).bits;
    }
    return result;
}
