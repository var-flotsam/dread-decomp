// behaviortree::CAutclastComputeAttackPositionAction's unit: fits `pos` onto the navigation mesh beside
// side `index` of a path. It probes the mesh across the side, then measures how far the floor side it
// hit runs straight each way (up to 300); with less than 300 in all it fails, else it moves `pos` so that
// 150 is left on the short side. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU55.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;

struct Vec2;
struct Result;
bool fn_7100ebfa7c(UnkNavMeshManager* self, Vec2* pos, Result* out, f32 a, u32 flags, f32 b);
Vec2* CNavMesh_GetVertex(UnkNavMesh* mesh, u16 index);
bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side);
struct UnkSource_7100286798;
u32 fn_7100413fd0(UnkSource_7100286798* self);

struct UnkAutclastPathPoint_l3b381c {
    CVector2D xy;
    f32 z;
};

struct UnkAutclastPath_l3b381c {
    UnkAutclastPathPoint_l3b381c* points;
    u32 count;
};

struct UnkAutclastAI_l3b381c {
    u8 pad_0x0[0x10];
    UnkSource_7100286798* field_0x10;
};

struct UnkAutclastOwner_l3b381c {
    u8 pad_0x0[0x240];
    UnkAutclastAI_l3b381c* ai;  // 0x240
};

struct UnkAutclastAction_l3b381c {
    u8 pad_0x0[0x18];
    UnkAutclastOwner_l3b381c* owner;  // 0x18
};

struct UnkProbePoint_l3b381c {
    f32 x;
    f32 y;
    UnkProbePoint_l3b381c(f32 x_, f32 y_) : x(x_), y(y_) {}
};

// A probe from `start` to `end` (the mesh query's segment).
struct UnkProbeSegment_l3b381c {
    UnkProbePoint_l3b381c start;
    UnkProbePoint_l3b381c end;
    UnkProbePoint_l3b381c delta;
    UnkProbeSegment_l3b381c(const UnkProbePoint_l3b381c& s, const UnkProbePoint_l3b381c& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

// What the query found: the cell and side hit and the point.
struct UnkProbeResult_l3b381c {
    u16 cell;
    u8 side;
    UnkProbePoint_l3b381c points[5];
    f32 distance;
    UnkProbeResult_l3b381c()
        : cell(0xffff), side(0xff),
          points{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, distance(0.0f) {}
};

static inline UnkNavMeshManager* NavMeshManager_l3b381c() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

// Side `index` of the closed path: from its point to the next one (the first after the last).
inline UnkSegment2D PathSide_l3b381c(const UnkAutclastPath_l3b381c* path, u32 index) {
    const UnkAutclastPathPoint_l3b381c* from = &path->points[index];
    const UnkAutclastPathPoint_l3b381c* to = index == path->count - 1 ? &path->points[0] : &path->points[index + 1];
    return UnkSegment2D(from->xy, to->xy);
}

inline f32 Distance_l3b381c(const CVector2D& a, const CVector2D& b) {
    f32 dx = a.x - b.x;
    f32 dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
}

inline UnkVec2Result Direction_l3b381c(const UnkSegment2D& segment) {
    return fn_7100125e78(&segment);
}

// A floor side close to horizontal or vertical.
inline bool RunsAlongAnAxis_l3b381c(const UnkVec2Result& dir) {
    return fabsf(dir.x) > 0.95f || fabsf(dir.y) > 0.95f;
}

inline f32 Dot_l3b381c(const UnkVec2Result& a, const UnkVec2Result& b) {
    return a.x * b.x + a.y * b.y;
}

inline const CVector2D& Vertex_l3b381c(UnkNavMesh* mesh, u16 index) {
    return *reinterpret_cast<const CVector2D*>(CNavMesh_GetVertex(mesh, index));
}

bool fn_71003b381c(UnkAutclastAction_l3b381c* self, u32 index, const UnkAutclastPath_l3b381c* path,
                   CVector2D* pos) {
    UnkNavMesh* mesh = NavMeshManager_l3b381c()->navmesh;

    // The path side's direction, and its normal.
    f32 dx, dy;
    {
        UnkSegment2D side = PathSide_l3b381c(path, index);
        dy = side.end.y - side.start.y;
        dx = side.end.x - side.start.x;
        f32 length = sqrtf(dy * dy + dx * dx);
        if (length != 0.0f) {
            f32 inv = 1.0f / length;
            dx *= inv;
            dy *= inv;
        }
    }
    UnkVec2Result normal = Direction_l3b381c(PathSide_l3b381c(path, index));
    u32 flags = fn_7100413fd0(self->owner->ai->field_0x10);

    // Probe the mesh across the side, 100 each way from `pos`.
    UnkProbeResult_l3b381c hit;
    f32 nx = normal.x * 100.0f;
    f32 ny = normal.y * 100.0f;
    UnkProbeSegment_l3b381c probe(UnkProbePoint_l3b381c(nx + pos->x, ny + pos->y),
                                  UnkProbePoint_l3b381c(pos->x - nx, pos->y - ny));
    if (!fn_7100ebfa7c(NavMeshManager_l3b381c(), reinterpret_cast<Vec2*>(&probe), reinterpret_cast<Result*>(&hit),
                       5.0f, 0, -1.0f))
        return false;
    u8 hit_side = hit.side;
    u16 hit_cell = hit.cell;
    fn_710007b444(pos, reinterpret_cast<const UnkVec3*>(&hit.points[0]));

    // How far the floor runs straight back from `pos` (towards the sides' starts).
    f32 back;
    {
        u8 side = hit_side;
        u16 cell = hit_cell;
        UnkNavMeshSidePair ends = fn_71002afe60(fn_710027a33c(mesh, cell), &side);
        UnkSegment2D edge(Vertex_l3b381c(mesh, ends.a), Vertex_l3b381c(mesh, ends.b));
        if (!RunsAlongAnAxis_l3b381c(fn_7100125e78(&edge)))
            return false;
        back = fabsf(Distance_l3b381c(edge.start, *pos));
        while (back <= 300.0f) {
            {
                CVector2D widths = {1.0f, 1.0f};
                if (!fn_710027c460(mesh, cell, side, false, &widths, flags, &cell, &side))
                    break;
            }
            ends = fn_71002afe60(fn_710027a33c(mesh, cell), &side);
            {
                UnkSegment2D next(Vertex_l3b381c(mesh, ends.a), Vertex_l3b381c(mesh, ends.b));
                fn_7100125b44(&edge, &next);
            }
            if (!(Dot_l3b381c(normal, fn_7100125e78(&edge)) > 0.99f))
                break;
            back += fn_7100125fbc(&edge);
        }
    }

    // And forward (towards the sides' ends).
    f32 front;
    {
        u8 side = hit_side;
        u16 cell = hit_cell;
        UnkNavMeshSidePair ends = fn_71002afe60(fn_710027a33c(mesh, cell), &side);
        UnkSegment2D edge(Vertex_l3b381c(mesh, ends.a), Vertex_l3b381c(mesh, ends.b));
        if (!RunsAlongAnAxis_l3b381c(fn_7100125e78(&edge)))
            return false;
        front = fabsf(Distance_l3b381c(edge.end, *pos));
        while (front <= 300.0f) {
            {
                CVector2D widths = {1.0f, 1.0f};
                if (!fn_710027c460(mesh, cell, side, true, &widths, flags, &cell, &side))
                    break;
            }
            ends = fn_71002afe60(fn_710027a33c(mesh, cell), &side);
            {
                UnkSegment2D next(Vertex_l3b381c(mesh, ends.a), Vertex_l3b381c(mesh, ends.b));
                fn_7100125b44(&edge, &next);
            }
            if (!(Dot_l3b381c(normal, fn_7100125e78(&edge)) > 0.99f))
                break;
            front += fn_7100125fbc(&edge);
        }
    }

    if (back + front < 300.0f)
        return false;
    if (back < 150.0f) {
        f32 shift = 150.0f - back;
        pos->x = dx * shift + pos->x;
        pos->y = dy * shift + pos->y;
    } else if (front < 150.0f) {
        f32 shift = 150.0f - front;
        pos->x = pos->x - dx * shift;
        pos->y = pos->y - dy * shift;
    }
    return true;
}
