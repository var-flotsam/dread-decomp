// Unit 2506: offers a candidate to a fixed list of 16 aim targets. A listed target with the same key is
// replaced when the candidate lies nearer to the point at +0x48c (result 1), or the offer is skipped when
// it lies farther; otherwise the candidate is appended (1, or 3 when the list is now full; 2 when it was
// already full). Names and layouts are placeholders.
#include <new>

#include "game/CAimComponent.h"
#include "game/UnkTU365.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};

static inline UnkVisitResult_b0040 Skip_ld4aa08() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

struct UnkKeyed_ld4aa08 {
    u8 pad_0x0[0x10];
    u64 key;  // 0x10
};

#pragma pack(push, 4)
struct UnkAimTargetArray_ld4aa08 {
    u32 count;
    UnkAimTarget items[16];
};
#pragma pack(pop)

struct UnkPoint_ld4aa08 {
    f32 x, y, z;
};

struct UnkAimTargets_ld4aa08 {
    void* vtable;
    UnkAimTargetArray_ld4aa08 array;  // 0x8
    UnkPoint_ld4aa08 point;  // 0x48c
};
static_assert(__builtin_offsetof(UnkAimTargets_ld4aa08, point) == 0x48c, "layout");

static inline u64 Key(const UnkAimTarget& target) {
    UnkKeyed_ld4aa08* owner = static_cast<UnkKeyed_ld4aa08*>(target.slot.ptr);
    return owner ? owner->key : 0;
}

static inline f32 DistanceSq(const UnkPoint_ld4aa08& p, const UnkAimTarget& target) {
    f32 dx = p.x - target.X();
    f32 dy = p.y - target.Y();
    f32 dz = p.z - target.Z();
    return dx * dx + dy * dy + dz * dz;
}

static inline f32 Sign(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

UnkVisitResult_b0040 fn_7100d4aa08(UnkAimTargets_ld4aa08* self, const UnkAimTarget* candidate) {
    u32 n = self->array.count;
    // The end is computed in bytes from the array's start (the original tests the byte size for zero).
    UnkAimTarget* end = reinterpret_cast<UnkAimTarget*>(reinterpret_cast<u8*>(&self->array) + (u64)n * sizeof(UnkAimTarget) + 4);
    for (UnkAimTarget* it = self->array.items; it != end; ++it) {
        UnkAimTarget& listed = *it;
        if (Key(*candidate) != Key(listed))
            continue;
        s32 order = (s32)Sign(DistanceSq(self->point, listed) - DistanceSq(self->point, *candidate));
        if (order > 0) {
            listed = *candidate;
            return UnkVisitResult_b0040(1);
        }
        if (order < 0)
            return Skip_ld4aa08();
    }
    s32 free = 16 - (s32)n;
    if (free > 0) {
        new (end) UnkAimTarget(*candidate);
        ++self->array.count;
        return UnkVisitResult_b0040(free == 1 ? 3 : 1);
    }
    return UnkVisitResult_b0040(2);
}
