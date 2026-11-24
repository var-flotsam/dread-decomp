// UnkTU811 (CEmmyAIComponent): how far the E.M.M.I. can go along its path. From the path point the movement is
// on, walks the navigation mesh along the side the point lies on, ahead and behind, as long as the sides stay
// in line; adds to `out` the room left (capped by the distance to a goal straight ahead). Returns false when
// the owner is not walking on a valid cell. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CEnemyMovement.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkPath442.h"
#include "game/UnkTU362.h"
#include "game/UnkTU363.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side);

struct UnkTracked_l550644 {
    virtual void vf_0x0();
    UnkNavMeshCell* Cell() {
        return reinterpret_cast<UnkNavMeshCell* (*)(UnkTracked_l550644*)>((*reinterpret_cast<void***>(this))[0x2d0 / 8])(this);
    }
};

struct UnkGoalA_l550644 {
    u8 pad_0x0[0x3810c];
    CVector3D position;  // 0x3810c
};

struct UnkGoalB_l550644 {
    u8 pad_0x0[0x3811c];
    CVector3D position;  // 0x3811c
};

struct UnkGoalKind_l550644 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkGoalEntry_l550644 {
    u8 pad_0x0[0x10];
    UnkGoalKind_l550644* kind;  // 0x10
};

struct UnkGoals_l550644 {
    UnkGoalEntry_l550644* entry;
    UnkGoalA_l550644* goal_a;  // 0x8
    u8 pad_0x10[0x28];
    UnkGoalB_l550644* goal_b;  // 0x38
};

struct UnkGoalHolder_l550644 {
    u8 pad_0x0[0xb0];
    UnkGoals_l550644* goals;  // 0xb0
};

struct UnkMovement_l550644 {
    u8 pad_0x0[0x974];
    CVector2D position;  // 0x974
    u8 pad_0x97c[0x8];
    u32 path_index;  // 0x984
};

struct UnkOwner_l550644 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1f8 - 0x78];
    UnkTracked_l550644* tracked;  // 0x1f8
    u8 pad_0x200[0x18];
    UnkMovement_l550644* movement;  // 0x218
    u8 pad_0x220[0x78];
    UnkGoalHolder_l550644* field_0x298;
};

struct UnkEmmyAI_l550644 {
    u8 pad_0x0[0x30];
    UnkOwner_l550644* owner;  // 0x30
    u8 pad_0x38[0x30];
    UnkPath442 path;  // 0x68
};

// A side's edge record as read here: its length and direction.
struct UnkEdge_l550644 {
    u8 pad_0x0[0x8];
    f32 length;  // 0x8
    f32 dx;      // 0xc
    f32 dy;      // 0x10
};

struct UnkCellSide_l550644 {
    s16 cell;
    u8 side;
};

// A point on a cell side (constructed from a path entry and a position).
struct UnkHit_l550644 {
    u16 cell;
    u8 side;
    CVector2D point;  // 0x4
    u8 pad_0xc[0x4];
    UnkHit_l550644(const u32* entry, const CVector2D* at) {
        fn_71002875bc(reinterpret_cast<UnkNavMeshHit*>(this), entry, at);
    }
    ~UnkHit_l550644() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};

// Where a walk along the sides ends and how long it is.
struct UnkWalk_l550644 {
    u16 cell;
    u8 side;
    CVector2D point;  // 0x4
    f32 length;       // 0xc
    UnkWalk_l550644() { fn_71002878dc(reinterpret_cast<UnkMsg_b152*>(this)); }
    ~UnkWalk_l550644() { fn_7100287954(reinterpret_cast<UnkMsg_b152*>(this)); }
};

struct UnkMeshCell_l550644 {
    u8 data[0x70];
};

struct UnkMeshCells_l550644 {
    u8 pad_0x0[0x290];
    UnkMeshCell_l550644* cells;  // 0x290
};

static inline UnkNavMeshCell* CellOf(UnkNavMesh* mesh, u16 id) {
    return reinterpret_cast<UnkNavMeshCell*>(&reinterpret_cast<UnkMeshCells_l550644*>(mesh)->cells[id]);
}

static inline const UnkEdge_l550644* EdgeOf(UnkNavMeshCell* cell, const u8* side) {
    return reinterpret_cast<const UnkEdge_l550644*>(fn_71002afbe4(cell, side));
}

static inline f32 Dot(const UnkEdge_l550644* a, const UnkEdge_l550644* b) {
    return a->dx * b->dx + a->dy * b->dy;
}

static inline UnkNavMeshManager* NavMeshManager() {
    UnkNavMeshManager* manager = g_unk_71726a4510;
    if (!manager) {
        manager = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(manager);
        g_unk_71726a4510 = manager;
    }
    return manager;
}

static inline const CVector3D* Goal(UnkOwner_l550644* owner) {
    UnkGoals_l550644* goals = owner->field_0x298->goals;
    if (!goals->entry || !goals->entry->kind)
        return nullptr;
    if (goals->entry->kind->kind == 1 && goals->goal_a)
        return &goals->goal_a->position;
    if (goals->entry->kind->kind == 7 && goals->goal_b)
        return &goals->goal_b->position;
    return nullptr;
}

bool fn_7100550644(UnkEmmyAI_l550644* self, f32* out) {
    *out = 200.0f;
    UnkMovement_l550644* movement = self->owner->movement;
    if (!movement || fn_71010cc3a4(movement) != 1)
        return false;
    if (!self->owner->tracked->Cell() || !fn_71002b005c(self->owner->tracked->Cell()))
        return false;

    CVector2D facing;
    {
        const UnkMatrix44& m = fn_710002614c(&self->owner->transform);
        CVector3D facing3 = {m.m[2], m.m[6], m.m[10]};
        fn_710007b430(&facing, &facing3);
    }

    // The distance to a goal straight ahead, or -1.
    f32 distance = -1.0f;
    if (const CVector3D* goal = Goal(self->owner)) {
        CVector2D goal2;
        CVector3D goal3 = {goal->x, goal->y, goal->z};
        fn_710007b430(&goal2, &goal3);
        f32 dx = goal2.x - movement->position.x;
        f32 dy = goal2.y - movement->position.y;
        f32 len = sqrtf(dx * dx + dy * dy);
        if (len != 0.0f) {
            dx /= len;
            dy /= len;
        }
        if (dx * facing.x + dy * facing.y > 0.99f) {
            f32 gx = goal2.x - movement->position.x;
            f32 gy = goal2.y - movement->position.y;
            distance = sqrtf(gx * gx + gy * gy);
        } else {
            distance = -1.0f;
        }
    }

    u32 index = movement->path_index;
    if (index >= self->path.SegmentCount())
        return true;
    const u32* entry = GetPathSegmentEntryOrFallback(&self->path, index);
    if (reinterpret_cast<const UnkCellSide_l550644*>(entry)->cell == -1)
        return true;

    f32 ahead_length;
    f32 behind_length;
    {
        UnkHit_l550644 hit(entry, &movement->position);
        UnkNavMesh* mesh = NavMeshManager()->navmesh;
        UnkNavMeshCell* cell = fn_710027a328(mesh, reinterpret_cast<const UnkCellSide_l550644*>(entry)->cell);
        UnkNavMeshSideSegment segment = {{0.0f, 0.0f}, {0.0f, 0.0f}, -1.0f};
        fn_71002afea8(cell, &hit.side, &segment);
        const UnkEdge_l550644* edge = EdgeOf(cell, &hit.side);
        f32 t = ((hit.point.x - segment.start.x) * edge->dx + (hit.point.y - segment.start.y) * edge->dy) /
                edge->length;
        f32 along = edge->dx * facing.x + edge->dy * facing.y;
        bool ahead_dir = along > 0.0f;
        bool behind_dir = !(along > 0.0f);

        // Ahead: along the sides in line with the edge, in the direction the owner faces.
        UnkWalk_l550644 ahead;
        if (hit.cell != 0xffff && hit.side != 0xff) {
            UnkNavMeshCell* cell = CellOf(mesh, hit.cell);
            const UnkEdge_l550644* first = EdgeOf(cell, &hit.side);
            UnkNavMeshSideSegment segment = {{0.0f, 0.0f}, {0.0f, 0.0f}, -1.0f};
            fn_71002afea8(cell, &hit.side, &segment);
            ahead.length = 0.0f;
            CVector2D point = {segment.start.x + t * (segment.end.x - segment.start.x),
                               segment.start.y + t * (segment.end.y - segment.start.y)};
            ahead.cell = hit.cell;
            ahead.side = hit.side;
            ahead.point = point;
            if (Dot(first, edge) > 0.99f) {
                CVector2D widths = {42.0f, 42.0f};
                ahead.point = ahead_dir ? segment.end : segment.start;
                ahead.length = (ahead_dir ? 1.0f - t : t) * first->length;
                UnkNavMeshSideSegment next_segment;
                const CVector2D& next_point = ahead_dir ? next_segment.end : next_segment.start;
                u16 next_cell = 0xffff;
                u8 next_side = 0xff;
                while (fn_710027c460(mesh, ahead.cell, ahead.side, ahead_dir, &widths, 0, &next_cell, &next_side)) {
                    UnkNavMeshCell* next = CellOf(mesh, next_cell);
                    const UnkEdge_l550644* side = EdgeOf(next, &next_side);
                    if (!(Dot(side, edge) > 0.99f))
                        break;
                    next_segment = {{0.0f, 0.0f}, {0.0f, 0.0f}, -1.0f};
                    fn_71002afea8(next, &next_side, &next_segment);
                    ahead.length = side->length + ahead.length;
                    ahead.cell = next_cell;
                    ahead.side = next_side;
                    ahead.point = next_point;
                    next_cell = 0xffff;
                    next_side = 0xff;
                }
            }
        }
        ahead_length = ahead.length;

        // Behind: the same, the other way.
        UnkWalk_l550644 behind;
        if (hit.cell != 0xffff && hit.side != 0xff) {
            UnkNavMeshCell* cell = CellOf(mesh, hit.cell);
            const UnkEdge_l550644* first = EdgeOf(cell, &hit.side);
            UnkNavMeshSideSegment segment = {{0.0f, 0.0f}, {0.0f, 0.0f}, -1.0f};
            fn_71002afea8(cell, &hit.side, &segment);
            behind.length = 0.0f;
            CVector2D point = {segment.start.x + t * (segment.end.x - segment.start.x),
                               segment.start.y + t * (segment.end.y - segment.start.y)};
            behind.cell = hit.cell;
            behind.side = hit.side;
            behind.point = point;
            if (Dot(first, edge) > 0.99f) {
                CVector2D widths = {42.0f, 42.0f};
                behind.point = behind_dir ? segment.end : segment.start;
                behind.length = (behind_dir ? 1.0f - t : t) * first->length;
                UnkNavMeshSideSegment next_segment;
                const CVector2D& next_point = behind_dir ? next_segment.end : next_segment.start;
                u16 next_cell = 0xffff;
                u8 next_side = 0xff;
                while (fn_710027c460(mesh, behind.cell, behind.side, behind_dir, &widths, 0, &next_cell, &next_side)) {
                    UnkNavMeshCell* next = CellOf(mesh, next_cell);
                    const UnkEdge_l550644* side = EdgeOf(next, &next_side);
                    if (!(Dot(side, edge) > 0.99f))
                        break;
                    next_segment = {{0.0f, 0.0f}, {0.0f, 0.0f}, -1.0f};
                    fn_71002afea8(next, &next_side, &next_segment);
                    behind.length = side->length + behind.length;
                    behind.cell = next_cell;
                    behind.side = next_side;
                    behind.point = next_point;
                    next_cell = 0xffff;
                    next_side = 0xff;
                }
            }
        }
        behind_length = behind.length;
    }

    f32 behind_room = behind_length + -468.0f;
    if (behind_room < 0.0f)
        return true;
    f32 ahead_room = ahead_length + -50.0f;
    if (ahead_room < 0.0f)
        return true;
    f32 room = ahead_room < behind_room ? ahead_room : behind_room;
    if (!(room < distance) && !(distance < 0.0f))
        room = distance;
    *out += room < 200.0f ? room : 200.0f;
    return true;
}
