// Unit 2510 (collision world): one collider swept by a 2D offset against another. Nothing when the offset
// is zero or the area the sweep covers misses the other's bounds; otherwise the shape pair is tested at the
// two transforms, through the caller's filter when one is given. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CCollider.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

// Returned in memory: its copy is user-written.
struct UnkContact {
    u32 value;

    UnkContact() { value = 0; }
    UnkContact(const UnkContact& other) { value = other.value; }
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

struct UnkShape;
struct UnkCollider;
struct UnkCollider_A5;
UnkMatrix44 fn_7100d36ac4(UnkCollider_A5* self);
UnkContact fn_7100d541c0(UnkCollider* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6,
                         bool flag);

// A shape's bounding box at a transform (its vf_0x28).
struct UnkShapeBox_d5925c {
    CVector3D min;
    CVector3D max;
};
struct UnkBoundedShape_d5925c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual UnkShapeBox_d5925c GetBounds(const UnkMatrix44* transform);  // vf_0x28
};

// A box projected onto the plane.
struct UnkArea_d5925c {
    CVector2D min;
    CVector2D max;

    explicit UnkArea_d5925c(const UnkShapeBox_d5925c& box) {
        fn_710007b430(&min, &box.min);
        fn_710007b430(&max, &box.max);
    }
};

// The collider as this test reads it: its bounds and its shape (+0x158).
struct UnkSweptCollider_d5925c {
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
    u8 pad_0x8[0x38];
    f32 minX;  // 0x40
    f32 minY;  // 0x44
    f32 maxX;  // 0x48
    f32 maxY;  // 0x4c
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
    u8 pad_0x61[0xf7];
    UnkBoundedShape_d5925c* shape;  // 0x158
};
static_assert(__builtin_offsetof(UnkSweptCollider_d5925c, shape) == 0x158, "UnkSweptCollider_d5925c layout");

// The filter the tester calls back for each candidate (vtable g_unk_7101aa8898).
struct UnkSweepFilter_d5925c {
    void** vtable;
    UnkSweptCollider_d5925c* node;
    UnkSweptCollider_d5925c* mover;
    void* filter;
    bool field_0x20;
    UnkSweepFilter_d5925c(UnkSweptCollider_d5925c* n, UnkSweptCollider_d5925c* m, void* f)
        : vtable(g_unk_7101aa8898 + 2), node(n), mover(m), filter(f), field_0x20(false) {}
};

// The sweep offset as the pair test takes it: a copy of the caller's, passed by value (in memory: its
// copy is user-written).
struct UnkOffset_d5925c {
    f32 x, y;
    explicit UnkOffset_d5925c(const CVector2D& v) : x(v.x), y(v.y) {}
    UnkOffset_d5925c(const UnkOffset_d5925c& o) : x(o.x), y(o.y) {}
};

// The pair test of the two shapes at their transforms, offset by the sweep.
static inline UnkContact Test(UnkShapeTester_A5_b68* tester, UnkShape* a, UnkMatrix44* ta, UnkShape* b,
                              UnkMatrix44* tb, UnkOffset_d5925c offset, UnkSweepFilter_d5925c* filter) {
    return fn_7100d541c0(reinterpret_cast<UnkCollider*>(tester), a, ta, b, tb, &offset, filter, false);
}

// NOTE: src/game/logic/collision/UnkCollisionWorld.cpp declares the result as its UnkColliderResult (the
// same 4-byte user-copied result); this returns fn_7100d541c0's UnkContact directly, as the original does.
UnkContact fn_7100d5925c(UnkCollider* mover, UnkCollider* target, const CVector2D* delta, void* filter) {
    UnkSweptCollider_d5925c* self = reinterpret_cast<UnkSweptCollider_d5925c*>(mover);
    UnkSweptCollider_d5925c* other = reinterpret_cast<UnkSweptCollider_d5925c*>(target);
    if (delta->x != 0.0f || delta->y != 0.0f) {
        UnkMatrix44 from = fn_7100d36ac4(reinterpret_cast<UnkCollider_A5*>(self));
        UnkMatrix44 to;
        UnkCopyMatrix(&to, from);
        f32 dx = delta->x;
        f32 dy = delta->y;
        to.m[3] = dx + from.m[3];
        to.m[7] = dy + from.m[7];
        to.m[11] = from.m[11] + 0.0f;

        UnkArea_d5925c area(self->shape->GetBounds(&from));
        {
            UnkArea_d5925c moved(self->shape->GetBounds(&to));
            area.min.x = area.min.x < moved.min.x ? area.min.x : moved.min.x;
            area.min.y = area.min.y < moved.min.y ? area.min.y : moved.min.y;
            area.max.x = area.max.x > moved.max.x ? area.max.x : moved.max.x;
            area.max.y = area.max.y > moved.max.y ? area.max.y : moved.max.y;
        }

        if (other->dirty)
            other->UpdateBounds();
        if (other->maxX >= area.min.x && other->minX <= area.max.x && other->maxY >= area.min.y &&
            other->minY <= area.max.y) {
            if (filter) {
                UnkSweepFilter_d5925c sweep_filter(other, self, filter);
                UnkShapeTester_A5_b68* tester = Tester();
                UnkShape* a = reinterpret_cast<UnkShape*>(self->shape);
                UnkMatrix44 ta = fn_7100d36ac4(reinterpret_cast<UnkCollider_A5*>(self));
                UnkShape* b = reinterpret_cast<UnkShape*>(other->shape);
                UnkMatrix44 tb = fn_7100d36a2c(reinterpret_cast<UnkShape*>(other));
                return Test(tester, a, &ta, b, &tb, UnkOffset_d5925c(*delta), &sweep_filter);
            }
            UnkShapeTester_A5_b68* tester = Tester();
            UnkShape* a = reinterpret_cast<UnkShape*>(self->shape);
            UnkMatrix44 ta = fn_7100d36ac4(reinterpret_cast<UnkCollider_A5*>(self));
            UnkShape* b = reinterpret_cast<UnkShape*>(other->shape);
            UnkMatrix44 tb = fn_7100d36a2c(reinterpret_cast<UnkShape*>(other));
            return Test(tester, a, &ta, b, &tb, UnkOffset_d5925c(*delta), nullptr);
        }
    }
    InitSkip();
    UnkContact skip;
    skip.value = g_unk_71726a4778.value;
    return skip;
}
