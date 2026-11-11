// Unit 437: the nearest-target visitor of the aim (as fn_710033265c in
// src/game/CAbilityShinespark/CAbilityShinespark_Glow_ovb0045.cpp keeps the nearest by distance):
// a candidate that passes the class filter at +0x78 replaces the best one (record at +0x8, flag at
// +0x70) when there is none or it lies nearer to the point at +0x78 (x, y, z squared on both sides).
// Result 1 when taken, else the guarded "skip" 0. Names and layouts are placeholders.
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);
struct UnkVisitResult_b0040 {
    s32 value;
    UnkVisitResult_b0040(s32 v) : value(v) {}
    UnkVisitResult_b0040(const UnkVisitResult_b0040& other) = default;
    ~UnkVisitResult_b0040() {}
};
extern UnkVisitResult_b0040 g_unk_71726a4778;  // the "skip" result
extern u64 g_unk_71726a4780;                   // its guard

static inline UnkVisitResult_b0040 Skip_ovb0198k() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

struct UnkAssignedSlot {
    void* ptr;

    UnkAssignedSlot& operator=(const UnkAssignedSlot& other);
};
void fn_71002a5d1c(UnkAssignedSlot* self, const UnkAssignedSlot* other);
__attribute__((always_inline)) inline UnkAssignedSlot& UnkAssignedSlot::operator=(const UnkAssignedSlot& other) {
    fn_71002a5d1c(this, &other);
    return *this;
}

struct UnkAimTarget_ovb0198k {
    UnkAssignedSlot slot;
    UnkAssignedSlot slot_0x8;
    u32 words[13];  // 0x10

    f32 X() const { return *reinterpret_cast<const f32*>(&words[1]); }
    f32 Y() const { return *reinterpret_cast<const f32*>(&words[2]); }

    UnkAimTarget_ovb0198k& operator=(const UnkAimTarget_ovb0198k& other) {
        if (this != &other) {
            slot = other.slot;
            slot_0x8 = other.slot_0x8;
            for (u32 i = 0; i < 13; ++i)
                words[i] = other.words[i];
        }
        return *this;
    }
};

struct UnkAimFilter_ovb0198k;
bool fn_7100354b34(UnkAimFilter_ovb0198k* self, UnkAimTarget_ovb0198k* target);

struct UnkAimPoint_ovb0198k {
    f32 x, y, z;
};

struct UnkAimNearest_ovb0198k {
    u8 pad_0x0[8];
    UnkAimTarget_ovb0198k best;  // 0x8
    u8 pad_0x50[0x20];
    bool has_best;  // 0x70
    u8 pad_0x71[7];
    UnkAimPoint_ovb0198k point;  // 0x78
};
static_assert(__builtin_offsetof(UnkAimNearest_ovb0198k, has_best) == 0x70, "layout");

static inline f32 Sign_ovb0198k(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

UnkVisitResult_b0040 fn_71003549a4(UnkAimNearest_ovb0198k* self, UnkAimTarget_ovb0198k* target) {
    if (fn_7100354b34(reinterpret_cast<UnkAimFilter_ovb0198k*>(&self->point), target)) {
        bool take = !self->has_best;
        if (!take) {
            f32 bx = self->best.X();
            f32 by = self->best.Y();
            f32 px = self->point.x;
            f32 py = self->point.y;
            f32 oldDy = py - by;
            f32 newDy = py - target->Y();
            f32 oldDx = px - bx;
            f32 newDx = px - target->X();
            f32 oldDyy = oldDy * oldDy;
            f32 oldDxx = oldDx * oldDx;
            f32 oldXy = oldDxx + oldDyy;
            f32 zz = self->point.z * self->point.z;
            f32 newDxx = newDx * newDx;
            f32 newDyy = newDy * newDy;
            f32 newXy = newDxx + newDyy;
            f32 oldDist = zz + oldXy;
            f32 newDist = zz + newXy;
            take = (s32)Sign_ovb0198k(oldDist - newDist) >= 1;
        }
        if (take) {
            self->has_best = true;
            self->best = *target;
            return UnkVisitResult_b0040(1);
        }
    }
    return Skip_ovb0198k();
}
