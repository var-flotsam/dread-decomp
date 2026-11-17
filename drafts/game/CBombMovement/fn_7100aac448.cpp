// CBombMovement's unit (tools/tu_map.py unit 1610): adds a target to the 32-target list. A listed target
// of the same holder is replaced when the new one lies nearer to the point at +0x90c (result 1), kept
// when farther (the "skip" result); otherwise the target is appended. Results: 1 added, 2 full, 3 added
// and now full. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/CAbilityShinespark.h"
#include "game/CAimComponent.h"
#include "game/UnkTU365.h"
#include "types.h"

#include <new>

using base::math::CVector3D;

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline UnkVisitResult_b0040 Skip_laac448() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

// What a target's first slot points at: its holder at +0x10.
struct UnkTargetOwner_laac448 {
    u8 pad_0x0[0x10];
    void* holder;  // 0x10
};

// A fixed-capacity list: a count, then room for N elements.
template <typename T, int N>
struct UnkFixedList_laac448 {
    u32 count;
    u8 storage[N * sizeof(T)];

    T* begin() { return reinterpret_cast<T*>(storage); }
    T* end() { return begin() + count; }
};

struct UnkTargetList_laac448 {
    u8 pad_0x0[0x8];
    UnkFixedList_laac448<UnkAimTarget, 32> targets;  // 0x8
    CVector3D point;                                 // 0x90c
};
static_assert(__builtin_offsetof(UnkTargetList_laac448, point) == 0x90c, "layout");

static inline void* HolderOf(const UnkAimTarget& target) {
    UnkTargetOwner_laac448* owner = static_cast<UnkTargetOwner_laac448*>(target.slot.ptr);
    return owner ? owner->holder : nullptr;
}

static inline f32 DistanceSq(const CVector3D& p, const UnkAimTarget& target) {
    f32 dx = p.x - target.X();
    f32 dy = p.y - target.Y();
    f32 dz = p.z - target.Z();
    return dx * dx + dy * dy + dz * dz;
}

static inline f32 Sign_laac448(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

UnkVisitResult_b0040 fn_7100aac448(UnkTargetList_laac448* self, const UnkAimTarget* target) {
    u32 count = self->targets.count;
    UnkAimTarget* end = self->targets.end();
    for (UnkAimTarget* listed = self->targets.begin(); listed != end; ++listed) {
        if (HolderOf(*listed) != HolderOf(*target))
            continue;
        s32 order = (s32)Sign_laac448(DistanceSq(self->point, *listed) - DistanceSq(self->point, *target));
        if (order > 0) {
            *listed = *target;
            return UnkVisitResult_b0040(1);
        }
        if (order < 0)
            return Skip_laac448();
    }
    s32 free = 32 - count;
    if (free > 0) {
        new (end) UnkAimTarget(*target);
        ++self->targets.count;
        return free == 1 ? 3 : 1;
    }
    return 2;
}
