// CChozoWarriorXUltimateGrabAttack unit: works out where the ultimate grab happens. The grab point and the
// owner's start lie on either side of the target, at the arena edges the Chozo Warrior X AI reports
// (fn_710049b2fc / fn_710049b360), the side chosen by where the owner stands; the start is then dropped onto
// the navigation mesh, and the hold points are set above it by the two colliders' heights.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassChozoWarriorXUltimateGrabAttack.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct Vec2;
struct Result;
bool fn_7100ebfa7c(UnkNavMeshManager* self, Vec2* pos, Result* out, f32 a, u32 flags, f32 b);
f32 fn_7100977c88(void*, char* owner);
f32 fn_710049b2fc(UnkChozoWarriorAI_b66a2* self);
f32 fn_710049b360(UnkChozoWarriorAI_b66a2* self);

// What the function fills: whether it is set, then eight points and directions.
struct UnkUltimateGrabPoints_l4885b0 {
    bool valid;
    UnkVec3f origin;   // 0x4
    UnkVec3f start;    // 0x10
    UnkVec3f ground;   // 0x1c
    UnkVec3f base;     // 0x28
    UnkVec3f grab;     // 0x34
    UnkVec3f back;     // 0x40
    UnkVec3f hold;     // 0x4c
    UnkVec3f forward;  // 0x58
};
static_assert(__builtin_offsetof(UnkUltimateGrabPoints_l4885b0, forward) == 0x58,
              "UnkUltimateGrabPoints_l4885b0 layout");

// The tunables of the attack, as read here.
struct UnkUltimateGrabTunable_l4885b0 {
    u8 pad_0x0[0x30];
    f32 edge_offset;  // 0x30
};

struct UnkUltimateGrabTracked_l4885b0;
struct UnkUltimateGrabTrackedVtbl_l4885b0 {
    u8 pad_0x0[0x2a8];
    char* (*vf_0x2a8)(UnkUltimateGrabTracked_l4885b0* self);
};

// What an actor tracks; vf_0x2a8 gives the collider fn_7100977c88 measures.
struct UnkUltimateGrabTracked_l4885b0 {
    UnkUltimateGrabTrackedVtbl_l4885b0* vtable;
    char* Collider() { return vtable->vf_0x2a8(this); }
};

struct UnkUltimateGrabAI_l4885b0;

struct UnkUltimateGrabActor_l4885b0 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x98[0x1f8 - 0x38 - sizeof(UnkXform)];
    UnkUltimateGrabTracked_l4885b0* tracked;  // 0x1f8
    u8 pad_0x200[0x40];
    UnkUltimateGrabAI_l4885b0* ai;  // 0x240
};
static_assert(__builtin_offsetof(UnkUltimateGrabActor_l4885b0, ai) == 0x240, "UnkUltimateGrabActor_l4885b0 layout");

struct UnkUltimateGrabAI_l4885b0 {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x28];
    UnkUltimateGrabActor_l4885b0* owner;  // 0x30
    u8 pad_0x38[0x149a4 - 0x38];
    f32 floor_y;  // 0x149a4
};
static_assert(__builtin_offsetof(UnkUltimateGrabAI_l4885b0, floor_y) == 0x149a4, "UnkUltimateGrabAI_l4885b0 layout");

struct UnkUltimateGrabPlayer_l4885b0 {
    u8 pad_0x0[0x208];
    UnkUltimateGrabTracked_l4885b0* tracked;  // 0x208
};

struct UnkUltimateGrabAttack_l4885b0 {
    void* vtable;
    UnkUltimateGrabActor_l4885b0* owner;  // 0x8
};

struct UnkUltimateGrabPoint2_l4885b0 {
    f32 x;
    f32 y;
    UnkUltimateGrabPoint2_l4885b0(f32 x_, f32 y_) : x(x_), y(y_) {}
};

struct UnkUltimateGrabSegment_l4885b0 {
    UnkUltimateGrabPoint2_l4885b0 start;
    UnkUltimateGrabPoint2_l4885b0 end;
    UnkUltimateGrabPoint2_l4885b0 delta;
    UnkUltimateGrabSegment_l4885b0(const UnkUltimateGrabPoint2_l4885b0& s, const UnkUltimateGrabPoint2_l4885b0& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

struct UnkUltimateGrabHit_l4885b0 {
    u16 cell;
    u8 side;
    UnkUltimateGrabPoint2_l4885b0 points[5];
    f32 distance;
    UnkUltimateGrabHit_l4885b0()
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

static inline UnkUltimateGrabAI_l4885b0* WarriorXAI(UnkUltimateGrabAI_l4885b0* ai) {
    return ai && fn_710009d50c(ai->vf_0x0(), fn_710048db44()) ? ai : nullptr;
}

static inline UnkVec3f Add(const UnkVec3f& a, const UnkVec3f& b) {
    return UnkVec3f(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline UnkVec3f Scale(const UnkVec3f& v, f32 s) {
    return UnkVec3f(v.x * s, v.y * s, v.z * s);
}

static inline f32 PosX(const UnkUltimateGrabActor_l4885b0* actor) {
    return actor->xform.m[0][3];
}

void fn_71004885b0(UnkUltimateGrabAttack_l4885b0* self, UnkUltimateGrabPoints_l4885b0* out, f32 reach) {
    UnkUltimateGrabAI_l4885b0* ai = WarriorXAI(self->owner->ai);
    UnkUltimateGrabTracked_l4885b0* tracked = self->owner->tracked;
    char* collider = tracked ? tracked->Collider() : nullptr;
    UnkUltimateGrabActor_l4885b0* target = reinterpret_cast<UnkUltimateGrabActor_l4885b0*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)));
    UnkUltimateGrabPlayer_l4885b0* player =
        reinterpret_cast<UnkUltimateGrabPlayer_l4885b0*>(fn_7100defcd4(g_unk_7101d47310));
    UnkUltimateGrabTracked_l4885b0* player_tracked = player ? player->tracked : nullptr;
    char* player_collider = player_tracked ? player_tracked->Collider() : nullptr;
    if (!ai)
        return;
    if (!collider)
        return;
    if (!target)
        return;
    if (!player_collider)
        return;

    // Which side of the target the owner grabs from; up on a ledge, the side of the edge it is past.
    f32 side = PosX(self->owner) - PosX(target);
    if (ai->owner->xform.m[1][3] > ai->floor_y + 50.0f) {
        f32 edge;
        fn_71010fc31c(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(ai), &edge);
        if (PosX(self->owner) < edge) {
            side = -1.0f;
        } else {
            fn_71010fc284(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(ai), &edge);
            if (PosX(self->owner) > edge)
                side = 1.0f;
        }
    }

    const UnkUltimateGrabTunable_l4885b0& tunable =
        reinterpret_cast<const UnkUltimateGrabTunable_l4885b0&>(g_unk_71726ce7d0);
    const UnkVec3f offset(0.0f, 0.0f, 0.0f);
    UnkVec3f grab = target->xform.GetTranslation();
    f32 start_x, start_y, start_z;
    UnkVec3f back;
    UnkVec3f forward;
    if (side >= 0.0f) {
        grab.x = fn_710049b360(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(ai)) + tunable.edge_offset;
        out->grab = Add(grab, offset);
        start_y = target->xform.m[1][3];
        start_z = target->xform.m[2][3];
        start_x = fn_710049b2fc(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(ai)) - tunable.edge_offset;
        back = UnkVec3f(1.0f, 0.0f, 0.0f);
        forward = UnkVec3f(-1.0f, 0.0f, 0.0f);
    } else {
        grab.x = fn_710049b2fc(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(ai)) - tunable.edge_offset;
        out->grab = Add(grab, offset);
        start_y = target->xform.m[1][3];
        start_z = target->xform.m[2][3];
        start_x = fn_710049b360(reinterpret_cast<UnkChozoWarriorAI_b66a2*>(ai)) + tunable.edge_offset;
        back = UnkVec3f(-1.0f, 0.0f, 0.0f);
        forward = UnkVec3f(1.0f, 0.0f, 0.0f);
    }
    out->back = back;
    out->valid = true;
    UnkVec3f origin = Add(UnkVec3f(start_x, start_y, start_z), offset);
    out->origin = origin;
    out->start = origin;
    out->base = origin;
    out->hold = origin;
    out->forward = forward;

    UnkUltimateGrabPoint2_l4885b0 from(origin.x, origin.y);
    UnkUltimateGrabPoint2_l4885b0 to(from.x, from.y - 10000.0f);
    UnkUltimateGrabSegment_l4885b0 segment(from, to);
    UnkUltimateGrabHit_l4885b0 hit;
    if (!fn_7100ebfa7c(NavMeshManager(), reinterpret_cast<Vec2*>(&segment), reinterpret_cast<Result*>(&hit), 60.0f,
                       0, -1.0f))
        return;

    const f32* p = &hit.points[0].x;
    out->ground = UnkVec3f(p[0], p[1], p[2]);
    out->base = out->ground;

    f32 owner_height = fn_7100977c88(tracked, collider);
    f32 player_height = fn_7100977c88(player_tracked, player_collider);
    out->hold = Add(out->base, UnkVec3f(0.0f, owner_height, 0.0f));
    out->grab = Add(Add(out->base, UnkVec3f(0.0f, player_height, 0.0f)), Scale(out->forward, reach));
}
