// CPullableGrapplePointComponent's unit (tools/tu_map.py, unit 1295): whether the grapple point can be pulled.
// The hit position (moved 5 units towards the actor when flag 0x20 is set) must pass fn_7100219588; then an
// actor whose CSamusMovement is in its kind-3 state passes only if fn_7100996edc accepts. Names and layouts are placeholders.
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::math::CVector3D;

struct UnkPullMoveState_l82cb24;

bool fn_7100996edc(UnkPullMoveState_l82cb24* state, void* owner, const void* hit);

struct UnkPullMoveState_l82cb24 {
    void* vtable;
    u8 pad_0x8[0x8];
    s32 kind;
};

struct UnkPullMovementVtbl_l82cb24 {
    const base::reflection::CType* (*GetClass)(void* self);
};

#pragma pack(push, 4)
struct UnkPullMovement_l82cb24 {
    UnkPullMovementVtbl_l82cb24* vtable;
    u8 pad_0x8[0xf9 - 0x8];
    bool disabled;  // 0xf9
    u8 pad_0xfa[0x2d08 - 0xfa];
    UnkPullMoveState_l82cb24* current;
    u8 pad_0x2d10[0x2d18 - 0x2d10];
    u32 state_count;
    UnkPullMoveState_l82cb24* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

struct UnkPullActor_l82cb24 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x1f0 - 0x68];
    UnkPullMovement_l82cb24* movement;  // 0x1f0
};

struct UnkPullHit_l82cb24 {
    u8 pad_0x0[0x14];
    CVector3D pos;  // 0x14
    u8 pad_0x20[0x3c - 0x20];
    u8 flags;  // 0x3c
};

struct UnkPullableGrapplePoint_l82cb24 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
};

static inline UnkPullMoveState_l82cb24* FindState(UnkPullMovement_l82cb24* self, s32 kind) {
    UnkPullMoveState_l82cb24** it = self->states;
    UnkPullMoveState_l82cb24** end = it + self->state_count;
    for (; it != end; ++it) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

bool fn_710082cb24(UnkPullableGrapplePoint_l82cb24* self, UnkPullActor_l82cb24* actor, const UnkPullHit_l82cb24* hit) {
    CVector3D pos = {hit->pos.x, hit->pos.y, hit->pos.z};
    if (hit->flags & 0x20) {
        CVector3D to_actor = CVector3D{actor->x, actor->y, actor->z} - pos;
        pos = Normalized(to_actor) * 5.0f + pos;
    }
    if (!fn_7100219588(g_unk_7101d47300, &pos, 0, 0))
        return false;
    UnkPullMovement_l82cb24* movement = actor->movement;
    if (!movement)
        return true;
    if (movement->disabled)
        return false;
    if (!fn_710009d50c(movement->vtable->GetClass(movement),
                       reinterpret_cast<const base::reflection::CType*>(fn_71009dda1c())))
        return true;
    if (fn_71009e12d0(movement))
        return false;
    UnkPullMoveState_l82cb24* state = FindState(movement, 3);
    if (!state || movement->current != state)
        return true;
    return fn_7100996edc(state, self->owner, hit);
}
