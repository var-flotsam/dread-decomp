// CAutclastChargeAttack unit (tools/tu_map.py unit 555): whether the wall the autclast stands on runs straight
// far enough both ways: it is walked back, then forward, round its corners, and fails where a wall turns
// away from the way the autclast faces. Names and layouts are placeholders.
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
struct UnkAutclastTunable_l3b4c7c {
    u8 pad_0x0[0x58];
    f32 wall_length;  // 0x58
};
extern UnkAutclastTunable_l3b4c7c g_unk_71726bd2c8;

// Where the autclast stands on the mesh: the cell, and the side of it it stands on.
struct UnkNavPlace_l3b4c7c {
    u16 cell;
    u8 side;
    u8 pad_0x3[0x9];

    ~UnkNavPlace_l3b4c7c() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }
};

struct UnkMoveState_l3b4c7c {
    u8 pad_0x0[0x8c];
    CVector2D position;  // 0x8c
};

struct UnkEnemyMovement_l3b4c7c {
    void** vtable;
    u8 pad_0x8[0x8e0];
    UnkMoveState_l3b4c7c state;  // 0x8e8
};

// The direction the movement faces.
UnkVec2Result fn_71010c9a1c(UnkEnemyMovement_l3b4c7c* self);

typedef const u16* (*GetCell_l3b4c7c)(UnkEnemyMovement_l3b4c7c*);
typedef UnkNavPlace_l3b4c7c (*GetPlace_l3b4c7c)(UnkEnemyMovement_l3b4c7c*);

struct UnkAutclastComponents_l3b4c7c {
    u8 pad_0x0[0x10];
    UnkReflObject* ai;  // 0x10
};

struct UnkAutclast_l3b4c7c {
    u8 pad_0x0[0x218];
    UnkEnemyMovement_l3b4c7c* movement;  // 0x218
    u8 pad_0x220[0x20];
    UnkAutclastComponents_l3b4c7c* components;  // 0x240
};

struct UnkAutclastChargeAttack_l3b4c7c {
    u8 pad_0x0[0x8];
    UnkAutclast_l3b4c7c* owner;  // 0x8
    u8 pad_0x10[0x48];
    UnkNavMesh* mesh;  // 0x58
};

// The wall along `side` of `cell`.
static inline UnkSegment2D SideSegment(UnkAutclastChargeAttack_l3b4c7c* self, u16 cell, u8* side) {
    UnkNavMeshSidePair pair = fn_71002afe60(fn_710027a328(self->mesh, cell), side);
    const CVector2D* a = CNavMesh_GetVertex(self->mesh, pair.a);
    const CVector2D* b = CNavMesh_GetVertex(self->mesh, pair.b);
    return UnkSegment2D(*a, *b);
}


// A point that copies word by word, like the game's vectors.
struct UnkPosition_l3b4c7c {
    f32 x;
    f32 y;

    explicit UnkPosition_l3b4c7c(const CVector2D& v) { *this = v; }
    UnkPosition_l3b4c7c& operator=(const CVector2D& v) {
        reinterpret_cast<a_u32*>(this)[0] = reinterpret_cast<const a_u32*>(&v)[0];
        reinterpret_cast<a_u32*>(this)[1] = reinterpret_cast<const a_u32*>(&v)[1];
        return *this;
    }
};

// The cell and side round the corner from `side` of `cell`, going forward or back; false at the end.
static inline bool NextSide(UnkAutclastChargeAttack_l3b4c7c* self, u16 cell, u8 side, bool forward, u32 flags,
                            u16* next_cell, u8* next_side) {
    CVector2D widths = {100.0f, 100.0f};
    return fn_710027c460(self->mesh, cell, side, forward, &widths, flags, next_cell, next_side);
}

bool fn_71003b4c7c(UnkAutclastChargeAttack_l3b4c7c* self) {
    UnkEnemyMovement_l3b4c7c* movement = self->owner->movement;
    UnkMoveState_l3b4c7c* state = fn_71010cc3a4(movement) == 1 ? &movement->state : nullptr;
    UnkPosition_l3b4c7c start(state->position);
    UnkVec2Result facing = fn_71010c9a1c(movement);
    u16 start_cell = *reinterpret_cast<GetCell_l3b4c7c>(movement->vtable[0x2d0 / 8])(movement);
    u8 start_side = reinterpret_cast<GetPlace_l3b4c7c>(movement->vtable[0x2d8 / 8])(movement).side;

    // Back along the wall, then forward: each way it must not turn from `facing` within the tunable length.
    for (s32 i = 0; i < 2; i++) {
        bool forward = i == 1;
        UnkPosition_l3b4c7c position = start;
        u16 cell = start_cell;
        u8 side = start_side;
        for (f32 remaining = g_unk_71726bd2c8.wall_length; remaining > 0.0f;) {
            UnkSegment2D wall = SideSegment(self, cell, &side);
            const CVector2D& corner = forward ? wall.end : wall.start;
            f32 dx = corner.x - position.x;
            f32 dy = corner.y - position.y;
            f32 to_corner = sqrtf(dx * dx + dy * dy);
            if (to_corner >= remaining)
                break;

            UnkReflObject* ai = self->owner->components->ai;
            UnkReflObject* ai_component = ai && fn_710009d50c(ai->vf_0x0(), fn_7100408b2c()) ? ai : nullptr;
            u32 flags = fn_7100413fd0(reinterpret_cast<UnkSource_7100286798*>(ai_component));
            bool straight = true;
            u8 next_side;
            u16 next_cell;
            if (NextSide(self, cell, side, forward, flags, &next_cell, &next_side)) {
                side = next_side;
                cell = next_cell;
                UnkSegment2D next_wall = SideSegment(self, cell, &side);
                position = forward ? next_wall.start : next_wall.end;
                UnkVec2Result normal = fn_7100125e78(&next_wall);
                straight = normal.x * facing.x + normal.y * facing.y > 0.99f;
            }
            remaining -= to_corner;
            if (!straight)
                return false;
        }
    }
    return true;
}