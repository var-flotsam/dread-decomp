// CInfesterAIComponent's unit (tools/tu_map.py, unit 892): whether the infester ball can roll from the
// movement's point along the navigation mesh's sides in `direction` and reach the target's body (a box the
// size of each side touches it) before its roll distance runs out. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkNavMeshHit.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU344.h"
#include "game/UnkTU362.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::reflection::UnkReflObject;

struct UnkPlayerMovementCache;
void* fn_71010d7a6c(UnkPlayerMovementCache* self);

struct Vec2;
using Mesh = UnkNavMesh;
Vec2* fn_710027a410(Mesh* mesh, u16 index);

// The next side along the mesh from side `side` of `cell`, in the given direction.
bool fn_710027c460(UnkNavMesh* mesh, u16 cell, u8 side, bool forward, const CVector2D* extents, u32 flags,
                   u16* out_cell, u8* out_side);
u32 fn_7100413fd0(UnkReflObject* ai);

base::reflection::UnkReflObject* fn_7101193ee0(UnkActorDefFactory* self, const CStrId* id);
struct UnkActorDefEntry345;
struct UnkActorDef345;
UnkActorDefEntry345* CActorDef_FindPartById(const UnkActorDef345* self, const u64* id);

__attribute__((visibility("hidden"))) extern CStrId g_unk_71726ec438;  // the infester ball's character class
__attribute__((visibility("hidden"))) extern u64 g_unk_71726ec440;     // its AI component's id

struct UnkInfesterBox_l5cbe00;
void fn_7100d68324(UnkInfesterBox_l5cbe00* self, const void* params);
void fn_7100d6845c(UnkInfesterBox_l5cbe00* self);

// A box placed by its centre, its rotation, its size and its angle.
struct UnkInfesterBoxParams_l5cbe00 {
    CVector2D center;
    f32 rotation[4];
    f32 width;
    f32 height;
    f32 angle;
    u32 field_0x24;
};

struct UnkInfesterBox_l5cbe00 {
    u8 raw[0xd8];

    explicit UnkInfesterBox_l5cbe00(const UnkInfesterBoxParams_l5cbe00* params) { fn_7100d68324(this, params); }
    ~UnkInfesterBox_l5cbe00() { fn_7100d6845c(this); }
};

static inline UnkNavMeshManager* UnkNavManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

static inline const CVector2D& Vertex(UnkNavMesh* mesh, u16 index) {
    return *reinterpret_cast<const CVector2D*>(fn_710027a410(mesh, index));
}

// Whether `value` is within 0.01 of `target`.
static inline bool Near(f32 value, f32 target) {
    return value - 0.01f <= target && value + 0.01f >= target;
}

// Whether a side is a wall facing the way the ball rolls: its normal points straight back.
static inline bool IsWall(const UnkSegment2D* side, u32 direction) {
    f32 back = direction == 1 ? -1.0f : 1.0f;
    return Near(fn_7100125e78(side).x, back) && Near(fn_7100125e78(side).y, 0.0f);
}

// The angle of a side from its normal, in quarter turns.
static inline f32 SideAngle(const UnkVec2Result& normal) {
    f32 angle;
    if (normal.x < -0.5f) {
        angle = 6.2831855f;
    } else if (normal.y > 0.5f) {
        angle = 1.5707964f;
    } else {
        angle = 0.0f;
        if (normal.y < -0.5f)
            angle = 4.712389f;
    }
    return angle;
}

struct UnkInfesterPathState_l5cbe00 {
    u8 pad_0x0[0x8c];
    CVector2D pos;  // 0x8c
};

// The movement: its current cell (vf_0x2d0) and hit (vf_0x2d8), and its path state at +0x8e8.
struct UnkInfesterMovement_l5cbe00 {
    void** vtable;
    u8 pad_0x8[0x8e0];
    UnkInfesterPathState_l5cbe00 state;  // 0x8e8

    const u16* Cell() { return reinterpret_cast<const u16* (*)(UnkInfesterMovement_l5cbe00*)>(vtable[0x2d0 / 8])(this); }
    UnkNavMeshHit Hit() { return reinterpret_cast<UnkNavMeshHit (*)(UnkInfesterMovement_l5cbe00*)>(vtable[0x2d8 / 8])(this); }
};

struct UnkInfesterOwner_l5cbe00;

// The ball's AI component: its roll distance (vf_0xb0, scaled by +0x50c).
struct UnkInfesterBallAI_l5cbe00 {
    void** vtable;
    u8 pad_0x8[0x504];
    f32 field_0x50c;

    f32 vf_0xb0(UnkInfesterOwner_l5cbe00* owner) {
        return reinterpret_cast<f32 (*)(UnkInfesterBallAI_l5cbe00*, UnkInfesterOwner_l5cbe00*)>(vtable[0xb0 / 8])(this, owner);
    }
};

struct UnkInfesterAISlot_l5cbe00 {
    u8 pad_0x0[0x10];
    UnkReflObject* component;  // +0x10
};

struct UnkInfesterOwner_l5cbe00 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // +0x38
    u8 pad_0x78[0x240 - 0x78];
    UnkInfesterAISlot_l5cbe00* ai;  // +0x240
};

struct UnkInfesterSelf_l5cbe00 {
    u8 pad_0x0[0x30];
    UnkInfesterOwner_l5cbe00* owner;  // +0x30
};

struct UnkInfesterTarget_l5cbe00 {
    u8 pad_0x0[0x200];
    UnkPlayerMovementCache* movement;  // +0x200
};

bool fn_71005cbe00(UnkInfesterSelf_l5cbe00* self, u32 direction, UnkInfesterMovement_l5cbe00* movement) {
    UnkInfesterTarget_l5cbe00* target =
        reinterpret_cast<UnkInfesterTarget_l5cbe00*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)));
    void* body = fn_71010d7a6c(target->movement);
    if (!body)
        return false;

    UnkReflObject* o = self->owner->ai->component;
    UnkReflObject* char_ai = o && fn_710009d50c(o->vf_0x0(), fn_7100408b2c()) ? o : nullptr;
    u32 flags = fn_7100413fd0(char_ai);
    UnkNavMesh* mesh = UnkNavManager()->navmesh;
    u16 cell = *movement->Cell();
    u8 side = movement->Hit().side;
    f32 sign = direction != 0 ? 1.0f : -1.0f;

    UnkInfesterPathState_l5cbe00* state = fn_71010cc3a4(movement) == 1 ? &movement->state : nullptr;
    f32 px = state->pos.x;
    f32 py = state->pos.y;

    UnkReflObject* found = fn_7101193ee0(g_unk_71726a0220, &g_unk_71726ec438);
    UnkReflObject* char_class = found && fn_710009d50c(found->vf_0x0(), fn_71011abbd4()) ? found : nullptr;
    UnkReflObject* comp = reinterpret_cast<UnkReflObject*>(
        CActorDef_FindPartById(reinterpret_cast<const UnkActorDef345*>(char_class), &g_unk_71726ec440));
    if (!comp)
        return false;
    if (!fn_710009d50c(comp->vf_0x0(), fn_71005cd300()))
        return false;
    UnkInfesterBallAI_l5cbe00* ball = reinterpret_cast<UnkInfesterBallAI_l5cbe00*>(comp);

    UnkNavMeshSidePair start_ends = fn_71002afe60(fn_710027a33c(mesh, cell), &side);
    UnkSegment2D start(Vertex(mesh, start_ends.a), Vertex(mesh, start_ends.b));
    if (IsWall(&start, direction))
        return false;
    if (Near(fn_710002614c(&self->owner->transform).m[5], -1.0f))
        return true;

    f32 budget = ball->vf_0xb0(self->owner) * ball->field_0x50c;
    while (budget > 0.0f) {
        UnkNavMeshSidePair ends = fn_71002afe60(fn_710027a33c(mesh, cell), &side);
        UnkSegment2D edge(Vertex(mesh, ends.a), Vertex(mesh, ends.b));
        const CVector2D& p = direction == 0 ? edge.start : edge.end;
        f32 dx = p.x - px;
        f32 dy = p.y - py;
        f32 length = sqrtf(dx * dx + dy * dy);
        if (IsWall(&edge, direction))
            return false;

        f32 half = sign * length * 0.5f;
        f32 ex = edge.end.x - edge.start.x;
        f32 ey = edge.end.y - edge.start.y;
        f32 side_length = sqrtf(ex * ex + ey * ey);
        if (side_length != 0.0f) {
            f32 inv = 1.0f / side_length;
            ex *= inv;
            ey *= inv;
        }
        f32 cx = px + half * ex;
        f32 cy = py + half * ey;
        UnkVec2Result normal = fn_7100125e78(&edge);
        f32 angle = SideAngle(normal);
        UnkInfesterBoxParams_l5cbe00 params;
        params.center.x = cx;
        params.center.y = cy;
        params.rotation[0] = cosf(angle);
        params.rotation[1] = sinf(angle);
        params.rotation[2] = -sinf(angle);
        params.rotation[3] = cosf(angle);
        params.width = 200.0f;
        params.height = length;
        params.angle = angle;
        params.field_0x24 = 0;
        UnkInfesterBox_l5cbe00 box(&params);
        if (fn_7100d57ca0(reinterpret_cast<UnkLavaDropsShape_A2_b71*>(&box), &g_unk_7101a1a680, body, nullptr, 0))
            return true;
        if (length >= budget) {
            f32 fx = edge.end.x - edge.start.x;
            f32 fy = edge.end.y - edge.start.y;
            f32 f_length = sqrtf(fx * fx + fy * fy);
            if (f_length != 0.0f) {
                f32 inv = 1.0f / f_length;
                fx *= inv;
                fy *= inv;
            }
            px += sign * (budget * fx);
            py += sign * (budget * fy);
            return false;
        }
        u16 next_cell;
        u8 next_side;
        bool moved;
        {
            CVector2D extents = {50.0f, 50.0f};
            moved = fn_710027c460(mesh, cell, side, direction == 1, &extents, flags, &next_cell, &next_side);
        }
        if (!moved) {
            budget -= length;
            return false;
        }
        cell = next_cell;
        side = next_side;
        UnkNavMeshSidePair next_ends = fn_71002afe60(fn_710027a33c(mesh, cell), &side);
        UnkSegment2D next(Vertex(mesh, next_ends.a), Vertex(mesh, next_ends.b));
        const CVector2D& q = direction == 1 ? next.start : next.end;
        px = q.x;
        py = q.y;
        budget -= length;
    }
    return false;
}
