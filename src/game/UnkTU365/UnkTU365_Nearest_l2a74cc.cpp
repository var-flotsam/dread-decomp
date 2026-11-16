// Unit 365 (tools/tu_map.py, hint navmesh::CNavMeshGenerator): a nearest-target visitor: a candidate replaces
// the best one when there is none or it lies nearer to the point at +0x58; at equal distance, when a direction
// is set, the one whose own direction (words 7 to 9) points further along it wins. Result 1 when taken, else
// the guarded "skip" 0. Names and layouts are placeholders.
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

static inline UnkVisitResult_b0040 Skip_l2a74cc() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_71726a4780), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_71726a4780)) {
            g_unk_71726a4778.value = 0;
            __cxa_guard_release(&g_unk_71726a4780);
        }
    }
    return g_unk_71726a4778;
}

struct UnkPoint3_l2a74cc {
    f32 x, y, z;
};

struct UnkNearest_l2a74cc {
    u8 pad_0x0[8];
    UnkAimTarget best;  // 0x8
    bool has_best;  // 0x50
    u8 pad_0x51[7];
    UnkPoint3_l2a74cc point;  // 0x58
    u8 pad_0x64[4];
    const UnkPoint3_l2a74cc* direction;  // 0x68, breaks ties when set
};
static_assert(__builtin_offsetof(UnkNearest_l2a74cc, direction) == 0x68, "layout");

static inline f32 Sign_l2a74cc(f32 v) { return (f32)((v > 0.0f) - (v < 0.0f)); }

// The target's own direction (words 7 to 9).
static inline f32 DirX(const UnkAimTarget& t) { return *reinterpret_cast<const f32*>(&t.words[7]); }
static inline f32 DirY(const UnkAimTarget& t) { return *reinterpret_cast<const f32*>(&t.words[8]); }
static inline f32 DirZ(const UnkAimTarget& t) { return *reinterpret_cast<const f32*>(&t.words[9]); }

UnkVisitResult_b0040 fn_71002a74cc(UnkNearest_l2a74cc* self, UnkAimTarget* target) {
    bool take = !self->has_best;
    if (!take) {
        f32 bx = self->best.X();
        f32 px = self->point.x;
        f32 by = self->best.Y();
        f32 py = self->point.y;
        f32 oldDy = py - by;
        f32 oldDx = px - bx;
        f32 newDx = px - target->X();
        f32 oldDyy = oldDy * oldDy;
        f32 oldDxx = oldDx * oldDx;
        f32 oldXy = oldDyy + oldDxx;
        f32 zz = self->point.z * self->point.z;
        f32 newDy = py - target->Y();
        f32 newDxx = newDx * newDx;
        f32 newDyy = newDy * newDy;
        f32 oldDist = zz + oldXy;
        f32 newXy = newDyy + newDxx;
        f32 newDist = newXy + zz;
        s32 order = (s32)Sign_l2a74cc(oldDist - newDist);
        const UnkPoint3_l2a74cc* dir = self->direction;
        if (dir && order == 0) {
            f32 bestAlong = DirX(self->best) * dir->x + DirY(self->best) * dir->y + DirZ(self->best) * dir->z;
            f32 newAlong = dir->x * DirX(*target) + dir->y * DirY(*target) + dir->z * DirZ(*target);
            order = newAlong > bestAlong;
        }
        take = order >= 1;
    }
    if (take) {
        self->has_best = true;
        self->best = *target;
        return UnkVisitResult_b0040(1);
    }
    return Skip_l2a74cc();
}
