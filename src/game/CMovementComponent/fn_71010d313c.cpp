// CMovementComponent unit: probes the navigation mesh (fn_7100ebfa7c) along the segment from the owner's
// position (+0x44/+0x54) in the horizontal direction of dir, reach long; on a hit stores the point found and
// the cell and side. Returns whether it hit. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct Vec2_ebfa7c;
struct Result;

struct UnkProbeActor_l0d313c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
};

struct UnkProbeMovement_l0d313c {
    u8 pad_0x0[0x30];
    UnkProbeActor_l0d313c* owner;  // 0x30
};

struct UnkProbePoint_l0d313c {
    f32 x;
    f32 y;
    UnkProbePoint_l0d313c(f32 x_, f32 y_) : x(x_), y(y_) {}
};

// Copies word by word, as UnkVec3f does.
struct UnkProbeStart_l0d313c {
    f32 x, y;
    explicit UnkProbeStart_l0d313c(const UnkProbeActor_l0d313c* a) {
        ((a_u32*)this)[0] = ((const a_u32*)&a->x)[0];
        ((a_u32*)this)[1] = ((const a_u32*)&a->y)[0];
    }
};

struct UnkProbeSegment_l0d313c {
    UnkProbePoint_l0d313c start;
    UnkProbePoint_l0d313c end;
    UnkProbePoint_l0d313c delta;
    UnkProbeSegment_l0d313c(const UnkProbePoint_l0d313c& s, const UnkProbePoint_l0d313c& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

struct UnkProbeResult_l0d313c {
    u16 cell;
    u8 side;
    UnkProbePoint_l0d313c points[5];
    f32 distance;
    UnkProbeResult_l0d313c()
        : cell(0xffff), side(0xff),
          points{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, distance(0.0f) {}
};

using base::math::CVector3D;

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
bool fn_71010d313c(UnkProbeMovement_l0d313c* self, const CVector3D* dir, CVector3D* out_point,
                   u16* out_cell, u8* out_side, f32 reach, f32 limit) {
    UnkProbeActor_l0d313c* o = self->owner;
    UnkProbeStart_l0d313c pos(o);
    f32 len = sqrtf(dir->x * dir->x + dir->y * dir->y + dir->z * dir->z);
    f32 inv = 1.0f / (len + 0.0001f);
    f32 ny = dir->y * inv;
    f32 nx = dir->x * inv;
    UnkProbePoint_l0d313c start(pos.x, pos.y);
    UnkProbePoint_l0d313c end(nx * reach + start.x, ny * reach + start.y);
    UnkProbeSegment_l0d313c segment(start, end);
    UnkProbeResult_l0d313c result;
    bool hit = fn_7100ebfa7c(NavMeshManager(), reinterpret_cast<Vec2_ebfa7c*>(&segment), reinterpret_cast<Result*>(&result),
                             limit, 0, -1.0f);
    if (hit) {
        const f32* p = &result.points[0].x;
        out_point->x = p[0];
        out_point->y = p[1];
        out_point->z = p[2];
        *out_cell = result.cell;
        *out_side = result.side;
    }
    return hit;
}
