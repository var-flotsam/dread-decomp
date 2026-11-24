// CAutclastChargeAttack unit: whether an autclast has room to charge from `from` along `dir`. It finds
// the navigation-mesh wall behind the start, then walks both ways along the wall, testing its body shape
// at each step, until a spot is found whose way forward is clear. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU54.h"
#include "game/UnkTU55.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};

// CTunableCharClassAutclastAIComponent's values read here.
struct UnkAutclastChargeTunable : UnkTunable {
    u8 pad_0x8[0x1c];
    u32 field_0x24;  // attempts along the wall
    u8 pad_0x28[0x4];
    f32 field_0x2c;  // body depth, and the distance walked per attempt
    f32 field_0x30;  // body length when charging up
    f32 field_0x34;  // body length when charging sideways
    f32 field_0x38;  // body length when charging down
    f32 field_0x3c;  // the smallest distance counted from the start
    u8 pad_0x40[0x14];
    f32 field_0x54;  // how far round a corner the walk may go
};
static_assert(__builtin_offsetof(UnkAutclastChargeTunable, field_0x54) == 0x54, "layout");
extern UnkTunable g_unk_71726bd2c8;

struct UnkPlayerMovementCache;
struct UnkSource_7100286798;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);
u32 fn_7100413fd0(UnkSource_7100286798* self);
const CVector2D* CNavMesh_GetVertex(UnkNavMesh* mesh, u16 index);

// What fn_7100d7c990 hands back: the target, with its movement cache at +0x200.
struct UnkAutclastTarget {
    u8 pad_0x0[0x200];
    UnkPlayerMovementCache* movement;
};

// The object at the actor's +0x240, holding its AI component at +0x10.
struct UnkAutclastComponents {
    u8 pad_0x0[0x10];
    UnkReflObject* ai;
};

// A ray for the navigation-mesh query: its two ends and the difference between them.
struct UnkNavRay {
    CVector2D start;
    CVector2D end;
    CVector2D delta;
    UnkNavRay(const CVector2D& s, const CVector2D& e) : start(s), end(e), delta{e.x - s.x, e.y - s.y} {}
};

// What the query found: the cell and side crossed, and where.
struct UnkNavHit {
    u16 cell;
    u8 side;
    CVector3D point;  // +0x4
    u32 field_0x10[8];
    UnkNavHit() : cell(0xffff), side(0xff), point{0.0f, 0.0f, 0.0f}, field_0x10{} {}
};
static_assert(sizeof(UnkNavHit) == 0x30, "layout");

bool fn_7100ebfa7c(UnkNavMeshManager* self, UnkNavRay* ray, UnkNavHit* out, f32 a, u32 flags, f32 b);

// Where the walk may continue past a corner (fn_710027c460).
struct UnkNavStepLimits {
    f32 depth;
    f32 min_distance;
};
bool fn_710027c460(UnkNavMesh* mesh, u16 cell, u8 side, bool forward, const UnkNavStepLimits* limits, u32 flags,
                   u16* out_cell, u8* out_side);

// A box placed at `pos` and turned by `angle`.
struct UnkBoxDesc {
    CVector2D pos;
    f32 rotation[4];
    f32 half_length;
    f32 half_depth;
    f32 angle;
    u32 field_0x24;
    UnkBoxDesc(const CVector2D& p, f32 a, f32 length, f32 depth)
        : pos{p.x, p.y}, rotation{cosf(a), sinf(a), -sinf(a), cosf(a)}, half_length(length), half_depth(depth),
          angle(a), field_0x24(0) {}
};

// The collision box built from a UnkBoxDesc.
struct UnkBoxShape {
    u8 data[0xd8];
    UnkBoxShape(const UnkBoxDesc* desc) asm("fn_7100d68324");
    ~UnkBoxShape() asm("fn_7100d6845c");
};

inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

inline UnkSegment2D SideSegment(UnkNavMesh* mesh, u16 cell, u8* side) {
    UnkNavMeshSidePair pair = fn_71002afe60(fn_710027a33c(mesh, cell), side);
    return UnkSegment2D(*CNavMesh_GetVertex(mesh, pair.a), *CNavMesh_GetVertex(mesh, pair.b));
}

bool fn_71003b514c(UnkActor* actor, const CVector2D* from, const CVector2D* dir) {
    UnkNavMesh* mesh = NavMeshManager()->navmesh;
    UnkAutclastTarget* target = reinterpret_cast<UnkAutclastTarget*>(fn_7100d7c990(actor));
    void* body = fn_71010d7a6c(target->movement);
    if (!body)
        return false;

    const UnkAutclastChargeTunable& tunable = static_cast<UnkAutclastChargeTunable&>(g_unk_71726bd2c8);
    CVector2D origin = {from->x, from->y};
    UnkNavHit hit;
    u32 attempts = tunable.field_0x24;
    UnkNavRay ray({dir->x * 100.0f + origin.x, dir->y * 100.0f + origin.y},
                  {origin.x - dir->x * 100.0f, origin.y - dir->y * 100.0f});
    if (!fn_7100ebfa7c(NavMeshManager(), &ray, &hit, 5.0f, 0, -1.0f))
        return false;

    u8 side = hit.side;
    u16 cell = hit.cell;
    fn_710007b444(&origin, reinterpret_cast<const UnkVec3*>(&hit.point));
    bool active[2] = {true, true};
    u8 sides[2] = {side, side};
    u16 cells[2] = {cell, cell};
    f32 depth = tunable.field_0x2c;
    CVector2D points[2] = {{origin.x, origin.y}, {origin.x, origin.y}};

    f32 length;
    if (fabsf(dir->x) > 0.9f)
        length = tunable.field_0x34;
    else if (dir->y < -0.9f)
        length = tunable.field_0x38;
    else
        length = tunable.field_0x30;
    f32 min_distance = tunable.field_0x3c;

    f32 angle;
    if (dir->x < -0.5f)
        angle = 6.2831855f;
    else if (dir->y > 0.5f)
        angle = 1.5707964f;
    else if (dir->y < -0.5f)
        angle = 4.712389f;
    else
        angle = 0.0f;

    UnkBoxDesc start_desc(origin, angle, length, depth);
    UnkBoxShape start_box(&start_desc);
    UnkVec3f offset(length * dir->x * 0.5f, length * dir->y * 0.5f, 0.0f);
    UnkMatrix44_7100078c9c transform = fn_7100078cd8(offset);

    for (u64 i = 0; i < 2; i++) {
        f32 sign = i == 0 ? -1.0f : 1.0f;
        for (u32 attempt = 0; attempt < attempts; attempt++) {
            if (!active[i])
                continue;
            f32 remaining = depth;
            while (remaining > 0.0f) {
                UnkSegment2D wall = SideSegment(mesh, cells[i], &sides[i]);
                const CVector2D& corner = i == 0 ? wall.start : wall.end;
                CVector2D& point = points[i];
                f32 dx = corner.x - point.x;
                f32 dy = corner.y - point.y;
                f32 to_corner = sqrtf(dx * dx + dy * dy);

                if (to_corner >= remaining) {
                    CVector2D along = {wall.end.x - wall.start.x, wall.end.y - wall.start.y};
                    f32 wall_length = sqrtf(along.x * along.x + along.y * along.y);
                    if (wall_length != 0.0f) {
                        f32 inv = 1.0f / wall_length;
                        along.x *= inv;
                        along.y *= inv;
                    }
                    point.x += sign * (remaining * along.x);
                    point.y += sign * (remaining * along.y);

                    CVector2D probe = point;
                    if (attempt == 0) {
                        CVector2D lead = i == 1 ? CVector2D{along.x * 20.0f, along.y * 20.0f}
                                                : CVector2D{along.x * -20.0f, along.y * -20.0f};
                        probe = {lead.x + point.x, lead.y + point.y};
                    }
                    UnkBoxDesc probe_desc(probe, angle, length, depth);
                    UnkBoxShape probe_box(&probe_desc);
                    if (!fn_7100d57ca0(reinterpret_cast<UnkLavaDropsShape_A2_b71*>(&probe_box), body, &transform,
                                       nullptr, 0))
                        break;

                    UnkNavRay ahead({point.x, point.y}, {point.x + length * dir->x, point.y + length * dir->y});
                    UnkNavHit wall_hit;
                    // Clear unless a wall stops the charge short.
                    bool clear = true;
                    if (fn_7100ebfa7c(NavMeshManager(), &ahead, &wall_hit, 50.0f, 0, -1.0f)) {
                        CVector2D reached;
                        fn_710007b430(&reached, &wall_hit.point);
                        f32 rx = reached.x - origin.x;
                        f32 ry = reached.y - origin.y;
                        f32 distance = sqrtf(rx * rx + ry * ry);
                        distance = distance > min_distance ? distance : min_distance;
                        clear = !(distance < length);
                    }
                    return clear;
                } else {
                    // Past the corner: follow the walls round it while they face away from `dir`.
                    UnkReflObject* ai = reinterpret_cast<UnkAutclastComponents*>(actor->field_0x240)->ai;
                    UnkReflObject* ai_component =
                        ai && fn_710009d50c(ai->vf_0x0(), fn_7100408b2c()) ? ai : nullptr;
                    u32 flags = fn_7100413fd0(reinterpret_cast<UnkSource_7100286798*>(ai_component));
                    f32 budget = tunable.field_0x54;
                    bool turned = false;
                    if (budget > 0.0f) {
                        u16 current = cells[i];
                        do {
                            UnkNavStepLimits limits = {depth, min_distance};
                            u16 next_cell;
                            u8 next_side;
                            if (!fn_710027c460(mesh, current, sides[i], i == 1, &limits, flags, &next_cell,
                                               &next_side))
                                break;
                            current = next_cell;
                            sides[i] = next_side;
                            UnkSegment2D next_wall = SideSegment(mesh, current, &sides[i]);
                            point = i == 1 ? next_wall.start : next_wall.end;
                            UnkVec2Result normal = fn_7100125e78(&next_wall);
                            if (normal.x * dir->x + normal.y * dir->y > 0.01f) {
                                turned = true;
                                break;
                            }
                            budget -= fn_7100125fbc(&next_wall);
                        } while (budget > 0.0f);
                        cells[i] = current;
                    }
                    remaining -= to_corner;
                    if (!turned) {
                        active[i] = false;
                        break;
                    }
                }
            }
        }
    }
    return false;
}
