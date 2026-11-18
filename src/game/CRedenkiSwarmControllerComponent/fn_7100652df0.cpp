// CRedenkiSwarmControllerComponent's unit: probes the navigation mesh between two points. When the way is
// clear, sets *steps to count + 3 and probes again along `dir` scaled by that many steps; on a hit there,
// cuts *steps to the fraction reached, but never below `count`. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkNavMeshManager.h"
#include "types.h"

// As src/game_hi/fn_7100ebfa7c.cpp defines it (its point and result types are local there).
struct Vec2;
struct Result;

extern f32 g_unk_7101a5aee0;

struct UnkNavPoint_l652df0 {
    f32 x;
    f32 y;
    UnkNavPoint_l652df0(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkNavPoint_l652df0(const UnkNavPoint_l652df0& o) : x(o.x), y(o.y) {}
};

// A segment with its direction (end - start).
struct UnkNavSegment_l652df0 {
    UnkNavPoint_l652df0 start;
    UnkNavPoint_l652df0 end;
    UnkNavPoint_l652df0 delta;
    UnkNavSegment_l652df0(const UnkNavPoint_l652df0& s, const UnkNavPoint_l652df0& e)
        : start(s), end(e), delta(end.x - start.x, end.y - start.y) {}
};

// fn_7100ebfa7c's result: no cell (0xffff), no side (0xff), points cleared.
struct UnkNavResult_l652df0 {
    u16 cell;
    u8 side;
    UnkNavPoint_l652df0 points[5];
    f32 distance;
    UnkNavResult_l652df0()
        : cell(0xffff), side(0xff),
          points{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, distance(0.0f) {}
};

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
bool fn_7100652df0(const UnkNavPoint_l652df0* pos, const UnkNavPoint_l652df0* target, f32 scale, u32 count,
                   const UnkNavPoint_l652df0* dir, u32* steps) {
    UnkNavSegment_l652df0 segment(*pos, *target);
    UnkNavResult_l652df0 result;
    if (fn_7100ebfa7c(NavMeshManager(), reinterpret_cast<Vec2*>(&segment), reinterpret_cast<Result*>(&result),
                      g_unk_7101a5aee0, 0, -1.0f))
        return false;

    u32 n = count + 3;
    *steps = n;
    f32 x = dir->x * scale * (f32)n;
    f32 y = dir->y * scale * (f32)n;
    UnkNavPoint_l652df0 end(pos->x + x, pos->y + y);
    UnkNavSegment_l652df0 ahead(*pos, end);
    if (fn_7100ebfa7c(NavMeshManager(), reinterpret_cast<Vec2*>(&ahead), reinterpret_cast<Result*>(&result),
                      g_unk_7101a5aee0, 0, -1.0f)) {
        u32 reached = (u32)(result.points[4].y * (f32)*steps);
        *steps = reached < count ? count : reached;
    }
    return true;
}
