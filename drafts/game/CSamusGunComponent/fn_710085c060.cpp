// CSamusGunComponent's unit (tools/tu_map.py unit 1315): adds a target to a ten-entry list, one entry per
// owner: a target whose owner is already listed replaces that entry when it lies nearer to the point after
// the list, and is skipped when farther. Results: 1 added or replaced, 2 full, 3 added and now full, else
// the guarded "skip". Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CAimComponent.h"
#include "game/UnkTU365.h"
#include "types.h"

#include <new>

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline UnkVisitResult_b0040 Skip_l85c060() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

using base::math::CVector3D;

struct UnkTargetOwner_l85c060 {
    u8 pad_0x0[0x10];
    void* id;  // 0x10
};

// The owner a target belongs to (none when its slot is empty).
static inline void* OwnerId(const UnkAimTarget& target) {
    UnkTargetOwner_l85c060* owner = static_cast<UnkTargetOwner_l85c060*>(target.slot.ptr);
    return owner ? owner->id : nullptr;
}

// A fixed-capacity list: a count, then room for N elements.
template <typename T, int N>
struct UnkFixedList_l85c060 {
    u32 count;
    u8 storage[N * sizeof(T)];

    T* begin() { return reinterpret_cast<T*>(storage); }
    T* end() { return begin() + count; }
};

struct UnkNearestTargets_l85c060 {
    void* vtable;
    UnkFixedList_l85c060<UnkAimTarget, 10> targets;  // 0x8
    CVector3D point;  // 0x2dc
};
static_assert(__builtin_offsetof(UnkNearestTargets_l85c060, point) == 0x2dc, "layout");

static inline f32 Sign_l85c060(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

static inline f32 DistanceSq(const CVector3D& point, const UnkAimTarget& target) {
    f32 dx = point.x - target.X();
    f32 dy = point.y - target.Y();
    f32 dz = point.z - target.Z();
    return dx * dx + dy * dy + dz * dz;
}

UnkVisitResult_b0040 fn_710085c060(UnkNearestTargets_l85c060* self, const UnkAimTarget* target) {
    for (UnkAimTarget* it = self->targets.begin(); it != self->targets.end(); ++it) {
        if (OwnerId(*target) != OwnerId(*it))
            continue;
        s32 order = (s32)Sign_l85c060(DistanceSq(self->point, *it) - DistanceSq(self->point, *target));
        if (order > 0) {
            *it = *target;
            return UnkVisitResult_b0040(1);
        }
        if (order < 0)
            return Skip_l85c060();
    }

    u32 count = self->targets.count;
    s32 free = 10 - (s32)count;
    if (free > 0) {
        new (&self->targets.begin()[count]) UnkAimTarget(*target);
        ++self->targets.count;
        return UnkVisitResult_b0040(free == 1 ? 3 : 1);
    }
    return UnkVisitResult_b0040(2);
}
