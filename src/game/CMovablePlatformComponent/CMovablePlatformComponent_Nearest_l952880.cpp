// CMovablePlatformComponent's unit (tools/tu_map.py unit 1431): a nearest-target visitor. A candidate whose
// direction (words 4 to 6) points along the visitor's direction (+0x94) by at least +0xa0, and whose slot
// object does not belong to the excluded owner (+0xa8), replaces the best one (record at +0x8, flag at +0x80)
// when there is none or it lies nearer to the point at +0x88. Result 1 when taken, else the guarded "skip" 0.
// Names and layouts are placeholders.
#include "base/math/CVector.h"
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

static inline UnkVisitResult_b0040 Skip_l952880() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

using base::math::CVector3D;

// The object a target's slot holds: its owner at +0x10.
struct UnkSlotObject_l952880 {
    u8 pad_0x0[0x10];
    void* owner;
};

struct UnkPlatformNearest_l952880 {
    u8 pad_0x0[8];
    UnkAimTarget best;  // 0x8
    u8 pad_0x50[0x30];
    bool has_best;  // 0x80
    u8 pad_0x81[7];
    CVector3D point;      // 0x88
    CVector3D direction;  // 0x94
    f32 min_along;        // 0xa0
    u8 pad_0xa4[4];
    void* excluded;  // 0xa8
};
static_assert(__builtin_offsetof(UnkPlatformNearest_l952880, excluded) == 0xa8, "layout");

static inline f32 Sign_l952880(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

// The target's own direction (words 4 to 6).
static inline f32 DirX(const UnkAimTarget& t) { return *reinterpret_cast<const f32*>(&t.words[4]); }
static inline f32 DirY(const UnkAimTarget& t) { return *reinterpret_cast<const f32*>(&t.words[5]); }
static inline f32 DirZ(const UnkAimTarget& t) { return *reinterpret_cast<const f32*>(&t.words[6]); }

static inline bool IsExcluded(const UnkPlatformNearest_l952880* self, const UnkAimTarget* target) {
    return self->excluded && target->slot.ptr &&
           static_cast<UnkSlotObject_l952880*>(target->slot.ptr)->owner == self->excluded;
}

UnkVisitResult_b0040 fn_7100952880(UnkPlatformNearest_l952880* self, UnkAimTarget* target) {
    f32 along = DirX(*target) * self->direction.x + DirY(*target) * self->direction.y +
                DirZ(*target) * self->direction.z;
    if (!(along < self->min_along) && !IsExcluded(self, target)) {
        bool take = !self->has_best;
        if (!take) {
            f32 px = self->point.x;
            f32 py = self->point.y;
            f32 oldDx = px - self->best.X();
            f32 oldDy = py - self->best.Y();
            f32 oldXy = oldDx * oldDx + oldDy * oldDy;
            f32 pz = self->point.z;
            f32 oldDz = pz - self->best.Z();
            f32 oldDist = oldXy + oldDz * oldDz;
            f32 newDx = px - target->X();
            f32 newDy = py - target->Y();
            f32 newXy = newDx * newDx + newDy * newDy;
            f32 newDz = pz - target->Z();
            f32 newDist = newXy + newDz * newDz;
            take = (s32)Sign_l952880(oldDist - newDist) >= 1;
        }
        if (take) {
            self->has_best = true;
            self->best = *target;
            return UnkVisitResult_b0040(1);
        }
    }
    return Skip_l952880();
}
