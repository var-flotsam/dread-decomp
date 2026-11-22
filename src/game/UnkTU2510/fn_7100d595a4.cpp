// Unit 2510 (collision world): one collider against a shape swept by a 2D offset. Nothing when the
// collider's bounds miss the sweep's or the offset is zero; otherwise the shape tester runs the sweep,
// through the caller's filter when one is given. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "game/UnkCollider.h"
#include "game/UnkShapeFilter_590ac.h"
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