// CMorphBallMovement's unit: keeps the nearest candidate (fn_7100314e54's visitor shape, with the
// distance test inline): a candidate replaces the best one (+0x8, flagged at +0x50) when it is closer
// in x and y to the point at +0x54 (z counted for both); answers 1 then, the "skip" result
// otherwise. Names and layouts are placeholders.
#include "types.h"

struct UnkAssignedSlot {
    void* ptr;

    UnkAssignedSlot& operator=(const UnkAssignedSlot& other);
};
void fn_71002a5d1c(UnkAssignedSlot* self, const UnkAssignedSlot* other);
__attribute__((always_inline)) inline UnkAssignedSlot& UnkAssignedSlot::operator=(const UnkAssignedSlot& other) {
    fn_71002a5d1c(this, &other);
    return *this;
}

struct UnkMorphBallTarget_00d8 {
    UnkAssignedSlot slot;
    UnkAssignedSlot slot_0x8;
    union {
        u32 words[13];  // 0x10
        struct {
            u32 w0;
            f32 x;  // 0x14
            f32 y;
        } v;
    };

    UnkMorphBallTarget_00d8& operator=(const UnkMorphBallTarget_00d8& other) {
        if (this != &other) {
            slot = other.slot;
            slot_0x8 = other.slot_0x8;
            for (u32 i = 0; i < 13; ++i)
                words[i] = other.words[i];
        }
        return *this;
    }
};

struct UnkMorphBallBest_00d8 {
    u8 pad_0x0[8];
    UnkMorphBallTarget_00d8 best;  // 0x8
    bool has_best;  // 0x50
    u8 pad_0x51[3];
    f32 x;  // 0x54
    f32 y;
    f32 z;
};

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

static inline UnkVisitResult_b0040 Skip_00d8() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

UnkVisitResult_b0040 fn_71009500d8(UnkMorphBallBest_00d8* self, const UnkMorphBallTarget_00d8* target) {
    if (self->has_best) {
        f32 ox = target->v.x;
        f32 oy = target->v.y;
        f32 sx = self->x;
        f32 sy = self->y;
        f32 bx0 = self->best.v.x;
        f32 by0 = self->best.v.y;
        f32 ty = sy - oy;
        f32 by = sy - by0;
        f32 tx = sx - ox;
        f32 bx = sx - bx0;
        f32 near = tx * tx + ty * ty;
        f32 zz = self->z * self->z;
        f32 far = bx * bx + by * by;
        f32 diff = near + zz - (zz + far);
        int order = (diff > 0.0f) - (diff < 0.0f);
        if (static_cast<int>(static_cast<f32>(order)) < 1)
            return Skip_00d8();
    }
    self->has_best = true;
    self->best = *target;
    return UnkVisitResult_b0040(1);
}
