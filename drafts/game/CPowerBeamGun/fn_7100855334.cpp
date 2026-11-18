// CPowerBeamGun's unit (tools/tu_map.py unit 1312): adds a target to a 128-entry list, one entry per owner:
// an entry with the same owner is replaced when the new target lies nearer to the point at +0x240c, and the
// new one is skipped when it lies farther. Results: 1 taken, 2 full, 3 added and now full, else the guarded
// "skip". Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CAimComponent.h"
#include "game/UnkTU365.h"
#include "types.h"

#include <algorithm>
#include <new>

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline UnkVisitResult_b0040 Skip_l855334() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

using base::math::CVector3D;

struct UnkTargetOwner_l855334 {
    u8 pad_0x0[0x10];
    void* holder;  // 0x10
};

// A fixed-capacity list: a count, then room for N elements.
template <typename T, int N>
struct UnkFixedList_l855334 {
    u32 count;
    u8 storage[N * sizeof(T)];

    T* begin() { return reinterpret_cast<T*>(storage); }
    T* end() { return begin() + count; }
};

struct UnkNearestList_l855334 {
    void* vtable;
    UnkFixedList_l855334<UnkAimTarget, 128> targets;  // 0x8
    CVector3D point;                                  // 0x240c
};
static_assert(__builtin_offsetof(UnkNearestList_l855334, point) == 0x240c, "layout");

static inline void* Owner_l855334(const UnkAimTarget& t) {
    UnkTargetOwner_l855334* owner = static_cast<UnkTargetOwner_l855334*>(t.slot.ptr);
    return owner ? owner->holder : nullptr;
}

static inline f32 Sign_l855334(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

static inline f32 DistanceSq_l855334(const CVector3D& p, const UnkAimTarget& t) {
    f32 dx = p.x - t.X();
    f32 dy = p.y - t.Y();
    f32 dz = p.z - t.Z();
    return dx * dx + dy * dy + dz * dz;
}

UnkVisitResult_b0040 fn_7100855334(UnkNearestList_l855334* self, const UnkAimTarget* target) {
    // The first entry of the same owner at another distance decides.
    s32 order = 0;
    UnkAimTarget* kept = std::find_if(self->targets.begin(), self->targets.end(), [&](const UnkAimTarget& entry) {
        if (Owner_l855334(entry) != Owner_l855334(*target))
            return false;
        order = (s32)Sign_l855334(DistanceSq_l855334(self->point, entry) - DistanceSq_l855334(self->point, *target));
        return order != 0;
    });
    if (kept != self->targets.end()) {
        if (order > 0) {
            *kept = *target;
            return UnkVisitResult_b0040(1);
        }
        return Skip_l855334();
    }

    u32 count = self->targets.count;
    s32 free = 128 - count;
    if (free > 0) {
        new (&self->targets.begin()[count]) UnkAimTarget(*target);
        ++self->targets.count;
        return free == 1 ? 3 : 1;
    }
    return 2;
}
