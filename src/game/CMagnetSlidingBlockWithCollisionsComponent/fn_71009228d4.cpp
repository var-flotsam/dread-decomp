// CMagnetSlidingBlockWithCollisionsComponent's unit (tools/tu_map.py unit 1413): a target visitor that keeps
// the nearest candidate (record at +0x8, flag at +0x70) to the point at +0x78, skipping candidates whose
// collision body is the ignored one at +0x88 or is attached elsewhere (+0x200). Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAimComponent.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::math::CVector3D;
using base::reflection::UnkReflObject;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline UnkVisitResult_b0040 Skip_l9228d4() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

struct UnkBody_l9228d4 {
    u8 pad_0x0[0x200];
    void* attached;  // 0x200
};

struct UnkCollision_l9228d4 : UnkReflObject {
    u8 pad_0x8[0x28];
    UnkBody_l9228d4* body;  // 0x30
};

struct UnkHolder_l9228d4 {
    u8 pad_0x0[0x10];
    UnkReflObject* object;  // 0x10
};

struct UnkOwner_l9228d4 {
    u8 pad_0x0[0x10];
    UnkHolder_l9228d4* holder;  // 0x10
};

struct UnkNearest_l9228d4 {
    u8 pad_0x0[8];
    UnkAimTarget best;  // 0x8
    u8 pad_0x50[0x20];
    bool has_best;  // 0x70
    u8 pad_0x71[7];
    CVector3D point;  // 0x78
    u8 pad_0x84[4];
    UnkBody_l9228d4* ignored;  // 0x88
};
static_assert(__builtin_offsetof(UnkNearest_l9228d4, ignored) == 0x88, "layout");

static inline f32 Sign_l9228d4(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

static inline bool IsExcluded(const UnkNearest_l9228d4* self, const UnkAimTarget* target) {
    UnkReflObject* object = static_cast<UnkOwner_l9228d4*>(target->slot.ptr)->holder->object;
    if (object && fn_710009d50c(object->vf_0x0(), fn_710085fc44())) {
        UnkBody_l9228d4* body = static_cast<UnkCollision_l9228d4*>(object)->body;
        return body && (body == self->ignored || body->attached);
    }
    return false;
}

UnkVisitResult_b0040 fn_71009228d4(UnkNearest_l9228d4* self, UnkAimTarget* target) {
    if (!IsExcluded(self, target)) {
        bool take = !self->has_best;
        if (!take) {
            f32 px = self->point.x;
            f32 py = self->point.y;
            f32 oldDx = px - self->best.X();
            f32 oldDy = py - self->best.Y();
            f32 oldDyy = oldDy * oldDy;
            f32 oldDxx = oldDx * oldDx;
            f32 oldXy = oldDyy + oldDxx;
            f32 pz = self->point.z;
            f32 oldDz = pz - self->best.Z();
            f32 oldDist = oldXy + oldDz * oldDz;
            f32 newDx = px - target->X();
            f32 newDy = py - target->Y();
            f32 newDyy = newDy * newDy;
            f32 newDxx = newDx * newDx;
            f32 newXy = newDyy + newDxx;
            f32 newDz = pz - target->Z();
            f32 newDist = newXy + newDz * newDz;
            take = (s32)Sign_l9228d4(oldDist - newDist) >= 1;
        }
        if (take) {
            self->has_best = true;
            self->best = *target;
            return UnkVisitResult_b0040(1);
        }
    }
    return Skip_l9228d4();
}
