// CMorphBallMovement's unit (tools/tu_map.py unit 1428): a target visitor that keeps one candidate
// (record at +0x8, flag at +0x50): a candidate replaces the kept one when there is none or it lies
// farther from the point at +0x54 (x, y, z squared on both sides). Result 1 when taken, else the
// guarded "skip" 0. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CAimComponent.h"
#include "game/UnkTU365.h"
#include "game/UnkVisitResult_b0040.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

static inline UnkVisitResult_b0040 Skip_l9500d8() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

using base::math::CVector3D;

struct UnkFarthest_l9500d8 {
    u8 pad_0x0[8];
    UnkAimTarget best;  // 0x8
    bool has_best;  // 0x50
    u8 pad_0x51[3];
    CVector3D point;  // 0x54
};
static_assert(__builtin_offsetof(UnkFarthest_l9500d8, point) == 0x54, "layout");

static inline f32 Sign_l9500d8(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

UnkVisitResult_b0040 fn_71009500d8(UnkFarthest_l9500d8* self, UnkAimTarget* target) {
    bool take = !self->has_best;
    if (!take) {
        f32 tx = target->X();
        f32 px = self->point.x;
        f32 ty = target->Y();
        f32 py = self->point.y;
        f32 newDy = py - ty;
        f32 newDx = px - tx;
        f32 oldDx = px - self->best.X();
        f32 newDyy = newDy * newDy;
        f32 newDxx = newDx * newDx;
        f32 newXy = newDyy + newDxx;
        f32 zz = self->point.z * self->point.z;
        f32 oldDy = py - self->best.Y();
        f32 oldDxx = oldDx * oldDx;
        f32 oldDyy = oldDy * oldDy;
        f32 newDist = zz + newXy;
        f32 oldXy = oldDyy + oldDxx;
        f32 oldDist = oldXy + zz;
        take = (s32)Sign_l9500d8(newDist - oldDist) >= 1;
    }
    if (take) {
        self->has_best = true;
        self->best = *target;
        return UnkVisitResult_b0040(1);
    }
    return Skip_l9500d8();
}
