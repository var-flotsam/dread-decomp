// CAutclastChargeAttack unit (tools/tu_map.py unit 555): walks the navigation-mesh wall the autclast stands
// on, one way or the other, round its corners, and tells whether the wall bends away from the way the
// autclast faces before the walk runs out. Names and layouts are placeholders.
#include <math.h>

#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkTU362.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;
using base::reflection::UnkReflObject;

struct UnkSource_7100286798;
u32 fn_7100413fd0(UnkSource_7100286798* self);
const CVector2D* CNavMesh_GetVertex(UnkNavMesh* mesh, u16 index);
bool fn_710027c460(UnkNavMesh* self, u16 cell, u8 side, bool forward, const CVector2D* widths, u32 flags,
                   u16* out_cell, u8* out_side);

// CTunableCharClassAutclastAIComponent's values read here.
struct UnkAutclastTunable_l3b5a14 {
    u8 pad_0x0[0x24];
    s32 steps;       // 0x24
    u8 pad_0x28[0x4];
    f32 step_length; // 0x2c
    u8 pad_0x30[0x24];
    f32 corner_length;  // 0x54
};
extern UnkAutclastTunable_l3b5a14 g_unk_71726bd2c8;

// Where the autclast stands on the mesh: the cell, and the side of it it stands on.
struct UnkNavPlace_l3b5a14 {
    u16 cell;
    u8 side;
    u8 pad_0x3[0x9];

    ~UnkNavPlace_l3b5a14() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};

struct UnkMoveState_l3b5a14 {
    u8 pad_0x0[0x8c];
    CVector2D position;  // 0x8c
};

struct UnkEnemyMovement_l3b5a14 {
    void** vtable;
    u8 pad_0x8[0x8e0];
    UnkMoveState_l3b5a14 state;  // 0x8e8
};

// The direction the movement faces.
UnkVec2Result fn_71010c9a1c(UnkEnemyMovement_l3b5a14* self);

typedef const u16* (*GetCell_l3b5a14)(UnkEnemyMovement_l3b5a14*);
typedef UnkNavPlace_l3b5a14 (*GetPlace_l3b5a14)(UnkEnemyMovement_l3b5a14*);

struct UnkAutclastComponents_l3b5a14 {
    u8 pad_0x0[0x10];
    UnkReflObject* ai;  // 0x10
};

struct UnkAutclast_l3b5a14 {
    u8 pad_0x0[0x218];
    UnkEnemyMovement_l3b5a14* movement;  // 0x218
    u8 pad_0x220[0x20];
    UnkAutclastComponents_l3b5a14* components;  // 0x240
};

struct UnkAutclastChargeAttack_l3b5a14 {
    u8 pad_0x0[0x8];
    UnkAutclast_l3b5a14* owner;  // 0x8
    u8 pad_0x10[0x48];
    UnkNavMesh* mesh;  // 0x58
};

// The wall along `side` of `cell`.
static inline UnkSegment2D SideSegment(UnkAutclastChargeAttack_l3b5a14* self, u16 cell, u8* side) {
    UnkNavMeshSidePair pair = fn_71002afe60(fn_710027a328(self->mesh, cell), side);
    const CVector2D* a = CNavMesh_GetVertex(self->mesh, pair.a);
    const CVector2D* b = CNavMesh_GetVertex(self->mesh, pair.b);
    return UnkSegment2D(*a, *b);
}


// A point that copies word by word, like the game's vectors.
struct UnkPosition_l3b5a14 {
    f32 x;
    f32 y;

    explicit UnkPosition_l3b5a14(const CVector2D& v) { *this = v; }
    UnkPosition_l3b5a14& operator=(const CVector2D& v) {
        reinterpret_cast<a_u32*>(this)[0] = reinterpret_cast<const a_u32*>(&v)[0];
        reinterpret_cast<a_u32*>(this)[1] = reinterpret_cast<const a_u32*>(&v)[1];
        return *this;
    }
};

// The cell and side round the corner from `side` of `cell`, going forward or back; false at the end.
static inline bool NextSide(UnkAutclastChargeAttack_l3b5a14* self, u16 cell, u8 side, bool forward, u32 flags,
                            u16* next_cell, u8* next_side) {
    CVector2D widths = {100.0f, 100.0f};
    return fn_710027c460(self->mesh, cell, side, forward, &widths, flags, next_cell, next_side);
}

bool fn_71003b5a14(UnkAutclastChargeAttack_l3b5a14* self, s32 direction) {
    UnkEnemyMovement_l3b5a14* movement = self->owner->movement;
    UnkMoveState_l3b5a14* state = fn_71010cc3a4(movement) == 1 ? &movement->state : nullptr;
    UnkPosition_l3b5a14 position(state->position);
    UnkVec2Result facing = fn_71010c9a1c(movement);
    u16 cell = *reinterpret_cast<GetCell_l3b5a14>(movement->vtable[0x2d0 / 8])(movement);
    u8 side = reinterpret_cast<GetPlace_l3b5a14>(movement->vtable[0x2d8 / 8])(movement).side;

    const UnkAutclastTunable_l3b5a14& tunable = g_unk_71726bd2c8;
    f32 remaining = tunable.step_length * (f32)(tunable.steps + 1);
    while (remaining > 0.0f) {
        UnkSegment2D wall = SideSegment(self, cell, &side);
        const CVector2D& corner = direction == 0 ? wall.start : wall.end;
        f32 dx = corner.x - position.x;
        f32 dy = corner.y - position.y;
        f32 to_corner = sqrtf(dx * dx + dy * dy);
        if (to_corner >= remaining)
            return false;

        f32 budget = tunable.corner_length;
        UnkReflObject* ai = self->owner->components->ai;
        UnkReflObject* ai_component = ai && fn_710009d50c(ai->vf_0x0(), fn_7100408b2c()) ? ai : nullptr;
        u32 flags = fn_7100413fd0(reinterpret_cast<UnkSource_7100286798*>(ai_component));
        u8 next_side;
        u16 next_cell;
        while (NextSide(self, cell, side, direction == 1, flags, &next_cell, &next_side)) {
            side = next_side;
            cell = next_cell;
            UnkSegment2D next_wall = SideSegment(self, cell, &side);
            position = direction == 1 ? next_wall.start : next_wall.end;
            UnkVec2Result normal = fn_7100125e78(&next_wall);
            if (normal.x * facing.x + normal.y * facing.y > 0.99f)
                break;
            budget -= fn_7100125fbc(&next_wall);
            if (!(budget > 0.0f))
                return true;
        }
        remaining -= to_corner;
    }
    return false;
}
