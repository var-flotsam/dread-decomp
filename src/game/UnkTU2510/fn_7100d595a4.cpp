// Unit 2510 (collision world): one collider against a shape swept by a 2D offset. Nothing when the
// collider's bounds miss the sweep's or the offset is zero; otherwise the shape tester runs the sweep,
// through the caller's filter when one is given. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkVisitResult_b0040.h"
#include "game/logic/collision/CCollider.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

static inline void InitSkip() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
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

struct UnkShapeFilter_590ac {
    void** vtable;
    UnkCollider* node;
    u64 field_0x10;
    void* filter;
    bool field_0x20;
    UnkShapeFilter_590ac(UnkCollider* n, void* f)
        : vtable(g_unk_7101aa8898 + 2), node(n), field_0x10(0), filter(f), field_0x20(false) {}
};

// The sweep's offset, handed to the tester as a copy.
struct UnkSweepDelta_595a4 {
    f32 x, y;
    UnkSweepDelta_595a4(const UnkSweepDelta_595a4& o) : x(o.x), y(o.y) {}
};

struct UnkSweepBox_595a4 {
    f32 minX, minY, maxX, maxY;
};

// The pair test of shape `a` (placed by `ta`) and shape `b` (placed by `tb`), with the sweep's offset handed over
// as a copy and the filter.
static inline UnkColliderResult Sweep(UnkShapeTester_A5_b68* tester, void* a, void* ta, void* b, const UnkMatrix44& tb,
                                      UnkSweepDelta_595a4 delta, UnkShapeFilter_590ac* filter, bool flag) {
    return fn_7100d541c0(tester, static_cast<UnkShape*>(a), ta, static_cast<UnkShape*>(b),
                         const_cast<UnkMatrix44*>(&tb), &delta, filter, flag);
}

UnkColliderResult fn_7100d595a4(void* a, void* b, UnkCollider* collider, void* c, void* d, void* e, bool flag) {
    const UnkSweepDelta_595a4* delta = static_cast<const UnkSweepDelta_595a4*>(c);
    const UnkSweepBox_595a4* box = static_cast<const UnkSweepBox_595a4*>(d);
    if (collider->dirty)
        collider->UpdateBounds();
    if (collider->maxX >= box->minX && collider->minX <= box->maxX && collider->maxY >= box->minY &&
        collider->minY <= box->maxY && !(delta->x == 0.0f && delta->y == 0.0f)) {
        if (e) {
            UnkShapeFilter_590ac filter(collider, e);
            UnkShapeTester_A5_b68* tester = Tester();
            return Sweep(tester, a, b, collider->owner, fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider)),
                                 *delta, &filter, flag);
        }
        UnkShapeTester_A5_b68* tester = Tester();
        return Sweep(tester, a, b, collider->owner, fn_7100d36a2c(reinterpret_cast<UnkShape*>(collider)),
                             *delta, nullptr, flag);
    }
    // Missed, or no motion: nothing to report.
    InitSkip();
    return UnkColliderResult(g_unk_71726a4778.value);
}