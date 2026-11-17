// CChozoCommanderKiGrabAttack unit: works out where the grab happens. When the owner's AI is the Chozo
// Commander's, the grab point lies between the two limits the AI keeps, pushed half the arena width against
// the way the owner faces; the point is then dropped onto the navigation mesh below it, and the two actors'
// hold points are set half their "Character" collider widths above it. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkAtExit.h"
#include "base/UnkMatrix44.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkNavMeshManager.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct Vec2;
struct Result;
bool fn_7100ebfa7c(UnkNavMeshManager* self, Vec2* pos, Result* out, f32 a, u32 flags, f32 b);

// What the function fills: whether it is set, then eight points and directions.
struct UnkGrabPoints_l431de4 {
    bool valid;
    UnkVec3f origin;       // 0x4
    UnkVec3f start;        // 0x10
    UnkVec3f ground;       // 0x1c
    UnkVec3f base;         // 0x28
    UnkVec3f target_hold;  // 0x34
    UnkVec3f back;         // 0x40
    UnkVec3f owner_hold;   // 0x4c
    UnkVec3f forward;      // 0x58
};
static_assert(__builtin_offsetof(UnkGrabPoints_l431de4, forward) == 0x58, "UnkGrabPoints_l431de4 layout");

struct UnkGrabAI_l431de4 {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x15d1c - 0x8];
    f32 limit_min;  // 0x15d1c
    u8 pad_0x15d20[4];
    f32 limit_max;  // 0x15d24
};

// A collider as read here: its bounds, refreshed through vf_0x98 while `dirty` is set.
struct UnkGrabCollider_l431de4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();

    u8 pad_0x8[0x38];
    f32 min_x;  // 0x40
    f32 min_y;  // 0x44
    f32 max_x;  // 0x48
    f32 max_y;  // 0x4c
    u8 pad_0x50[0x10];
    bool dirty;  // 0x60
};
static_assert(__builtin_offsetof(UnkGrabCollider_l431de4, dirty) == 0x60, "UnkGrabCollider_l431de4 layout");

struct UnkGrabActor_l431de4 {
    u8 pad_0x0[0x38];
    u8 transform[0xc];  // 0x38
    f32 x;              // 0x44
    u8 pad_0x48[0x1f8];
    UnkGrabAI_l431de4* ai;  // 0x240
    u8 pad_0x248[0x58];
    UnkCollisionContainer* collision;  // 0x2a0
};
static_assert(__builtin_offsetof(UnkGrabActor_l431de4, collision) == 0x2a0, "UnkGrabActor_l431de4 layout");

struct UnkGrabAttack_l431de4 {
    void* vtable;
    UnkGrabActor_l431de4* owner;  // 0x8
};

struct UnkGrabPoint2_l431de4 {
    f32 x;
    f32 y;
    UnkGrabPoint2_l431de4(f32 x_, f32 y_) : x(x_), y(y_) {}
};

struct UnkGrabSegment_l431de4 {
    UnkGrabPoint2_l431de4 start;
    UnkGrabPoint2_l431de4 end;
    UnkGrabPoint2_l431de4 delta;
    UnkGrabSegment_l431de4(const UnkGrabPoint2_l431de4& s, const UnkGrabPoint2_l431de4& e)
        : start(s), end(e), delta(e.x - s.x, e.y - s.y) {}
};

struct UnkGrabHit_l431de4 {
    u16 cell;
    u8 side;
    UnkGrabPoint2_l431de4 points[5];
    f32 distance;
    UnkGrabHit_l431de4()
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

// Half the width of the actor's collider named `name`, or 0 when it has none.
inline f32 HalfWidth(UnkGrabActor_l431de4* actor, const CStrId& name) {
    if (!actor->collision)
        return 0.0f;
    UnkGrabCollider_l431de4* collider =
        reinterpret_cast<UnkGrabCollider_l431de4*>(fn_71008640a4(actor->collision, &name));
    if (!collider)
        return 0.0f;
    if (collider->dirty)
        collider->vf_0x98();
    f32 dx = collider->max_x - collider->min_x;
    f32 dy = collider->max_y - collider->min_y;
    return sqrtf(dx * dx + dy * dy) * 0.5f;
}

void fn_7100431de4(UnkGrabAttack_l431de4* self, UnkGrabPoints_l431de4* out) {
    UnkGrabActor_l431de4* target =
        reinterpret_cast<UnkGrabActor_l431de4*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)));

    UnkGrabAI_l431de4* ai = self->owner->ai;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710044e708())) {
        out->origin.x = ai->limit_min + (ai->limit_max - ai->limit_min) * 0.5f;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->transform));
        f32 facing = m.m[2];
        s32 sign = (facing > 0.0f) - (facing < 0.0f);
        UnkVec3f forward(1.0f, 0.0f, 0.0f);
        out->back = UnkVec3f(-forward.x, -forward.y, -forward.z);
        out->forward = forward;
        out->origin.x = sign * -600.0f * 0.5f + out->origin.x;
        out->start = out->origin;
        out->base = out->origin;
        out->target_hold = out->origin;
        out->owner_hold = out->origin;
        out->valid = true;
    }

    UnkGrabPoint2_l431de4 from(out->start.x, out->start.y);
    UnkGrabPoint2_l431de4 to(from.x, from.y - 10000.0f);
    UnkGrabSegment_l431de4 segment(from, to);
    UnkGrabHit_l431de4 hit;
    if (!fn_7100ebfa7c(NavMeshManager(), reinterpret_cast<Vec2*>(&segment), reinterpret_cast<Result*>(&hit), 60.0f, 0,
                       -1.0f))
        return;

    const f32* p = &hit.points[0].x;
    out->ground = UnkVec3f(p[0], p[1], p[2]);
    out->base = out->ground;

    static CStrId s_character("Character", true);
    f32 owner_half = HalfWidth(self->owner, s_character);
    f32 target_half = HalfWidth(target, s_character);

    out->target_hold = UnkVec3f(out->base.x + 0.0f, target_half + out->base.y - 20.0f, out->base.z + 0.0f);
    out->owner_hold = UnkVec3f(out->base.x + 0.0f, owner_half + out->base.y - 20.0f, out->base.z + 0.0f);

    f32 dir = self->owner->x - target->x >= 0.0f ? -1.0f : 1.0f;
    UnkVec3f forward(dir, 0.0f, 0.0f);
    out->forward = forward;
    out->back = UnkVec3f(-forward.x, -forward.y, -forward.z);
    out->valid = true;
}
