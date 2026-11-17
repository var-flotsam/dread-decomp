// Unit 1128 (tools/tu_map.py): whether an enemy's movement should act now: on reaching the current waypoint of
// the scenario's route (state 1), at random distances along its path segment, or (state 4) when its planned route
// passes the waypoint's cell. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/CEnemyMovement.h"
#include "game/UnkObject_0x100.h"
#include "game/UnkPath442.h"
#include "game/UnkTU376.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;

extern "C" int rand();
struct UnkStruct_71003609b8_Target;
UnkStruct_71003609b8_Target* fn_71010cc3bc(void* obj);

struct UnkActorWaypoints_l705cec;
bool fn_71007080dc(UnkActorWaypoints_l705cec* self);

// A cell of the navigation mesh and the side of it.
struct UnkNavCell_l705cec {
    u16 cell;
    u8 side;
    u8 pad_0x3;
};

// A planned route (fn_71002b57bc builds one, fn_71002b5c90 assigns one, fn_71002b6094 destroys one).
struct UnkRoute_l705cec {
    u8 pad_0x0[0x2];
    s16 field_0x2;
    u8 pad_0x4[0x1a];
    s16 field_0x1e;
    u8 pad_0x20[0x60];
    UnkNavCell_l705cec* cells;  // 0x80
    u32 cell_count;             // 0x88
    u8 pad_0x8c[0x74];

    UnkRoute_l705cec() { fn_71002b57bc(reinterpret_cast<UnkObject_0x100*>(this)); }
    ~UnkRoute_l705cec() { fn_71002b6094(reinterpret_cast<UnkObject_0x100*>(this)); }
    UnkRoute_l705cec& operator=(const UnkRoute_l705cec& other) {
        fn_71002b5c90(reinterpret_cast<UnkObject_0x100*>(this), reinterpret_cast<const UnkObject_0x100*>(&other));
        return *this;
    }
};
static_assert(sizeof(UnkRoute_l705cec) == 0x100, "layout");

struct UnkMovement_l705cec {
    u8 pad_0x0[0x974];
    f32 x;              // 0x974
    f32 y;              // 0x978
    u8 pad_0x97c[0x8];
    u32 segment;        // 0x984
    u8 pad_0x988[0xee8];
    UnkRoute_l705cec route;  // 0x1870
    bool has_route;     // 0x1970
    u8 pad_0x1971[0x32];
    bool field_0x19a3;
};
static_assert(__builtin_offsetof(UnkMovement_l705cec, field_0x19a3) == 0x19a3, "layout");

// The AI's path: its points (UnkPath442's layout).
struct UnkAIPath_l705cec {
    CVector3D* points;  // 0x0
    u32 count;          // 0x8
    u8 pad_0xc[0x20];
    u8 closed;          // 0x2c

    u32 SegmentCount() const { return count - ((closed == 0) & (count != 0)); }
    const UnkPath442* AsPath() const { return reinterpret_cast<const UnkPath442*>(this); }
};

struct UnkAI_l705cec {
    void** vtable;
    u8 pad_0x8[0x60];
    UnkAIPath_l705cec path;  // 0x68

    // vtable slot 0x6e8
    bool vf_0x6e8() { return reinterpret_cast<bool (*)(UnkAI_l705cec*)>(vtable[0xdd])(this); }
};

struct UnkActor_l705cec {
    u8 pad_0x0[0x218];
    UnkMovement_l705cec* movement;  // 0x218
    u8 pad_0x220[0x20];
    UnkAI_l705cec* ai;              // 0x240
};

struct UnkWaypoint_l705cec {
    u8 pad_0x0[0x8];
    UnkNavCell_l705cec cell;  // 0x8
    u8 pad_0xc[0xd];
    u8 side;                  // 0x19
    u16 field_0x1a;
    u8 pad_0x1c[0x1a];
    s16 field_0x36;
    u8 pad_0x38[0x20];
    bool field_0x58;
    f32 min_from_start;       // 0x5c
    f32 min_to_end;           // 0x60
    u8 pad_0x64[0xbc];
};
static_assert(sizeof(UnkWaypoint_l705cec) == 0x120, "layout");

struct UnkWaypointList_l705cec {
    void** vtable;
    u32 field_0x8;
    // Room for 530 waypoints, constructed in place.
    alignas(UnkWaypoint_l705cec) u8 storage[530 * sizeof(UnkWaypoint_l705cec)];  // 0xc

    UnkWaypoint_l705cec& operator[](u32 i) { return reinterpret_cast<UnkWaypoint_l705cec*>(storage)[i]; }
    // vtable slot 0x28
    u32 Count() { return reinterpret_cast<u32 (*)(UnkWaypointList_l705cec*)>(vtable[5])(this); }
};

struct UnkScenario_l705cec {
    u8 pad_0x0[0x12ac8];
    UnkWaypointList_l705cec list;  // 0x12ac8
    u8 pad_0x37f18[0xb0];
    u32 current;                   // 0x37fc8
    CVector3D target;              // 0x37fcc
    u8 pad_0x37fd8[0x4];
    bool field_0x37fdc;
};
static_assert(__builtin_offsetof(UnkScenario_l705cec, current) == 0x37fc8, "layout");
static_assert(__builtin_offsetof(UnkScenario_l705cec, field_0x37fdc) == 0x37fdc, "layout");

struct UnkActorWaypoints_l705cec {
    u8 pad_0x0[0x8];
    f32 factor;                     // 0x8
    UnkActor_l705cec* owner;        // 0x10
    UnkScenario_l705cec* scenario;  // 0x18
    bool reached;                   // 0x20
};

static inline f32 Random01() {
    return (rand() & 0x7fff) * (1.0 / 32767.0);
}

bool fn_7100705cec(UnkActorWaypoints_l705cec* self, f32 range, f32 offset) {
    UnkMovement_l705cec* movement = self->owner->movement;
    if (!movement)
        return false;

    if (fn_71010cc3a4(movement) == 1) {
        if (self->reached) {
            self->reached = false;
            return true;
        }
        if (offset + 0.1f >= 0.0f && offset - 0.1f <= 0.0f)
            return true;

        UnkScenario_l705cec* scenario = self->scenario;
        u32 index = scenario->current;
        if (index == ~0u)
            return false;
        UnkWaypointList_l705cec& waypoints = scenario->list;
        CVector3D target = {scenario->target.x, scenario->target.y, scenario->target.z};
        f32 distance;
        {
            CVector2D at;
            fn_710007b430(&at, &target);
            f32 dx = at.x - movement->x;
            f32 dy = at.y - movement->y;
            distance = sqrtf(dx * dx + dy * dy);
        }
        bool near;
        if (waypoints[index].field_0x1a == 0xffff)
            near = distance <= range;
        else
            near = distance <= range && waypoints[index].field_0x36 == -1;
        if (!self->owner)
            return near;
        UnkAI_l705cec* ai = self->owner->ai;
        if (waypoints[index].field_0x1a == 0xffff || near || !ai)
            return near;
        if (waypoints[index].field_0x36 == -1 || !waypoints[index].field_0x58)
            return false;
        if (movement->segment > ai->path.SegmentCount())
            return false;
        const UnkNavCell_l705cec* cell =
            reinterpret_cast<const UnkNavCell_l705cec*>(fn_710035e580(ai->path.AsPath(), movement->segment));
        if (cell->cell == 0xffff || cell->side == 0xff)
            return false;
        if (cell->cell != waypoints[index].field_0x1a || cell->side != waypoints[index].side)
            return false;

        // Act at a random fraction of the distance to the nearer end of the path segment.
        u32 segment = movement->segment;
        CVector3D* points = ai->path.points;
        UnkSegment2D line(reinterpret_cast<const CVector2D&>(points[segment]),
                          reinterpret_cast<const CVector2D&>(
                              ai->path.count - 1 == segment ? points[0] : points[segment + 1]));
        f32 length = fn_710035e544(ai->path.AsPath(), movement->segment);
        f32 dx = movement->x - line.start.x;
        f32 dy = movement->y - line.start.y;
        f32 travelled = sqrtf(dx * dx + dy * dy);
        if (!(travelled >= waypoints[index].min_from_start))
            return false;
        f32 remaining = length - travelled;
        if (!(remaining >= waypoints[index].min_to_end))
            return false;
        f32 sx = target.x - line.start.x;
        f32 sy = target.y - line.start.y;
        f32 ex = target.x - line.end.x;
        f32 ey = target.y - line.end.y;
        bool towards_start = sx * sx + sy * sy < ex * ex + ey * ey;
        f32 left = towards_start ? travelled : remaining;
        f32 minimum = towards_start ? waypoints[index].min_from_start : waypoints[index].min_to_end;
        if (!(minimum > self->factor * left))
            return false;
        self->factor = Random01() * 0.3f + 0.5f;
        return true;
    }

    if (!fn_71010cc3bc(movement))
        return false;
    if (fn_71010cc3a4(movement) != 4)
        return false;
    if (!movement->field_0x19a3) {
        if (!fn_71007080dc(self))
            return false;
        self->scenario->field_0x37fdc = true;
        return true;
    }
    if (self->reached || !self->owner)
        return false;
    UnkAI_l705cec* ai = self->owner->ai;
    if (!ai)
        return false;
    if (!ai->vf_0x6e8())
        return false;
    UnkScenario_l705cec* scenario = self->scenario;
    u32 index = scenario->current;
    if (index == ~0u || index != scenario->list.Count() - 1)
        return false;
    // Whether the planned route passes the last waypoint's cell.
    UnkRoute_l705cec route;
    if (movement->has_route)
        route = movement->route;
    if (movement->has_route && route.field_0x2 != -1 && route.field_0x1e != -1 && route.cell_count != 0) {
        UnkWaypointList_l705cec& waypoints = scenario->list;
        const UnkNavCell_l705cec* end = route.cells + route.cell_count;
        for (const UnkNavCell_l705cec* c = route.cells; c != end; ++c) {
            if (c->cell == waypoints[index].cell.cell && c->side == waypoints[index].cell.side) {
                self->reached = true;
                break;
            }
        }
    }
    return false;
}
