// CChozoCommanderUltimateGrabAttack's unit (tools/tu_map.py, unit 658): plans the grab's path: from the
// grabbed actor's position, facing away from the target, `reach` ahead; then, when the navigation mesh
// finds the ground below the position, lifts the path's two points above it by the heights of the owner's
// and the player's bodies. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "types.h"

f32 CCharacterMovement_GetNegatedShapeBoundsMinY(void*, char* owner);

// A body: its vtable slot 0x2a8 gives what its height is measured from.
struct UnkGrabBody_l43f9cc {
    void** vtable;
    char* vf_0x2a8() { return reinterpret_cast<char* (*)(UnkGrabBody_l43f9cc*)>(vtable[0x2a8 / 8])(this); }
};

struct UnkGrabPlayer_l43f9cc {
    u8 pad_0x0[0x208];
    UnkGrabBody_l43f9cc* body;  // 0x208
};

// What the commander's breakable part holds: the actor it grabbed, at +0x10.
struct UnkGrabTarget_l43f9cc {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

// The commander's AI component.
struct UnkGrabAI_l43f9cc {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x15d30 - 0x38];
    u8 part[8];  // 0x15d30, a breakable part

    const base::reflection::CType* vf_0x0() {
        return reinterpret_cast<const base::reflection::CType* (*)(UnkGrabAI_l43f9cc*)>(vtable[0])(this);
    }
    UnkGrabTarget_l43f9cc* Target() {
        return reinterpret_cast<UnkGrabTarget_l43f9cc*>(*fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(part)));
    }
};

struct UnkUltimateGrab_l43f9cc {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
};

// The planned path.
struct UnkGrabPlan_l43f9cc {
    bool valid;
    UnkVec3f start;     // 0x4
    UnkVec3f from;      // 0x10
    UnkVec3f hit;       // 0x1c
    UnkVec3f ground;    // 0x28
    UnkVec3f target;    // 0x34
    UnkVec3f back;      // 0x40
    UnkVec3f lifted;    // 0x4c
    UnkVec3f facing;    // 0x58
};
static_assert(sizeof(UnkGrabPlan_l43f9cc) == 0x64, "plan");

// A segment of the navigation mesh's plane: its ends and the step from one to the other.
struct UnkNavSegment_l43f9cc {
    base::math::CVector2D from;
    base::math::CVector2D to;
    base::math::CVector2D step;

    // Straight down from `start`, `depth` long.
    UnkNavSegment_l43f9cc(UnkVec3f start, f32 depth) {
        from.x = start.x;
        from.y = start.y;
        to.x = start.x;
        to.y = start.y - depth;
        step.x = to.x - from.x;
        step.y = to.y - from.y;
    }
};

// Where a segment meets the mesh: the cell and side crossed and the point.
struct UnkNavHit_l43f9cc {
    u16 cell;
    u8 side;
    UnkVec3f point;  // 0x4
    u32 field_0x10[8];

    UnkNavHit_l43f9cc() : cell(0xffff), side(0xff), point(0.0f, 0.0f, 0.0f), field_0x10{} {}
};
static_assert(sizeof(UnkNavHit_l43f9cc) == 0x30, "hit");

bool fn_7100ebfa7c(UnkNavMeshManager* self, const UnkNavSegment_l43f9cc* segment, UnkNavHit_l43f9cc* hit,
                   u32 flags, f32 radius, f32 limit);

static inline UnkVec3f Add(UnkVec3f a, UnkVec3f b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline UnkVec3f Scale(UnkVec3f v, f32 s) {
    return UnkVec3f(v.x * s, v.y * s, v.z * s);
}

static inline UnkVec3f Negate(UnkVec3f v) {
    return UnkVec3f(-v.x, -v.y, -v.z);
}

static inline UnkNavMeshManager* NavManager() {
    if (!g_unk_71726a4510) {
        UnkNavMeshManager* m = static_cast<UnkNavMeshManager*>(fn_71000839a0(0x62ec8, 0x3f));
        fn_7100ebf278(m);
        g_unk_71726a4510 = m;
    }
    return g_unk_71726a4510;
}

void fn_710043f9cc(UnkUltimateGrab_l43f9cc* self, UnkGrabPlan_l43f9cc* plan, f32 reach) {
    UnkGrabAI_l43f9cc* ai = static_cast<UnkGrabAI_l43f9cc*>(self->owner->field_0x240);
    if (ai && !fn_710009d50c(ai->vf_0x0(), fn_710044e708()))
        ai = nullptr;
    UnkGrabBody_l43f9cc* body = static_cast<UnkGrabBody_l43f9cc*>(self->owner->field_0x1f8);
    char* own_base = body ? body->vf_0x2a8() : nullptr;
    UnkActor* target = fn_7100d7c990(self->owner);
    UnkGrabPlayer_l43f9cc* player = reinterpret_cast<UnkGrabPlayer_l43f9cc*>(fn_7100defcd4(g_unk_7101d47310));
    UnkGrabBody_l43f9cc* player_body = player ? player->body : nullptr;
    char* player_base = player_body ? player_body->vf_0x2a8() : nullptr;
    if (!ai || !own_base || !target || !player_base)
        return;

    f32 dir = self->owner->field_0x44 - target->field_0x44 >= 0.0f ? -1.0f : 1.0f;
    UnkGrabTarget_l43f9cc* grab = ai->Target();
    UnkActor* grabbed = grab && grab->actor ? ai->Target()->actor : ai->actor;
    UnkVec3f pos(grabbed->field_0x44, grabbed->field_0x54, grabbed->field_0x64);
    UnkVec3f facing(dir, 0.0f, 0.0f);
    UnkVec3f offset = Scale(facing, reach);
    plan->start = pos;
    plan->from = pos;
    plan->ground = pos;
    plan->lifted = pos;
    plan->back = Negate(facing);
    plan->valid = true;
    plan->target = Add(offset, pos);
    plan->facing = facing;

    UnkNavSegment_l43f9cc down(pos, 10000.0f);
    UnkNavHit_l43f9cc hit;
    if (!fn_7100ebfa7c(NavManager(), &down, &hit, 0, 60.0f, -1.0f))
        return;
    plan->hit = hit.point;
    plan->ground = hit.point;
    f32 x = plan->back.x;
    UnkVec3f side((f32)((x > 0.0f) - (x < 0.0f)) * 300.0f, 0.0f, 0.0f);
    UnkVec3f own_lift = Add(side, UnkVec3f(0.0f, CCharacterMovement_GetNegatedShapeBoundsMinY(body, own_base), 0.0f));
    UnkVec3f player_lift = Add(side, UnkVec3f(0.0f, CCharacterMovement_GetNegatedShapeBoundsMinY(player_body, player_base), 0.0f));
    plan->lifted = Add(plan->ground, own_lift);
    plan->target = Add(offset, Add(plan->ground, player_lift));
}
