// behaviortree::CInfesterFleeFromTargetAction's unit (tools/tu_map.py unit 888): the update: waits for the
// actor's movement, flees from the target and, after a delay or once the target is inside its reach, makes
// the actor react (a kind 5 parameter block and a request to its char class). Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::reflection::UnkReflObject;

struct UnkPolyline108_b67a1;
bool fn_7100118be8(const UnkPolyline108_b67a1* self);
bool fn_710035e53c(const UnkPolyline108_b67a1* self);

struct UnkInfesterActor_l5c885c {
    u8 pad_0x0[0x1f0];
    UnkReflObject* movement;  // 0x1f0
    u8 pad_0x1f8[0x20];
    void* field_0x218;
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkInfesterAI_l5c885c : UnkReflObject {
    u8 pad_0x8[0x60];
    u8 path[0x11b8];  // 0x68
    bool field_0x1220;
    u8 pad_0x1221[0x134bc];
    bool field_0x146dd;
};
static_assert(__builtin_offsetof(UnkInfesterAI_l5c885c, field_0x146dd) == 0x146dd, "layout");

struct UnkInfesterStateLeaf_l5c885c {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
};

struct UnkInfesterStateNode_l5c885c {
    u8 pad_0x0[0x10];
    UnkInfesterStateLeaf_l5c885c* leaf;  // 0x10
};

struct UnkInfesterState_l5c885c {
    UnkInfesterStateNode_l5c885c* node;
    u8 pad_0x8[0x20];
    void* field_0x28;
};

struct UnkInfesterReactor_l5c885c {
    u8 pad_0x0[0xb0];
    UnkInfesterState_l5c885c* state;  // 0xb0
};

struct UnkInfesterBounds_l5c885c {
    u8 pad_0x0[0x8c];
    f32 x;  // 0x8c
    f32 y;  // 0x90
};

struct UnkInfesterMovement_l5c885c : UnkReflObject {
    u8 pad_0x8[0x8e0];
    UnkInfesterBounds_l5c885c field_0x8e8;
};

struct UnkInfesterFleeFromTargetAction_l5c885c {
    u8 pad_0x0[0x18];
    UnkInfesterActor_l5c885c* actor;  // 0x18
    u8 pad_0x20[0x10];
    UnkInfesterAI_l5c885c* ai;             // 0x30
    UnkInfesterReactor_l5c885c* reactor;   // 0x38
    UnkInfesterBounds_l5c885c* bounds;     // 0x40
    CVector3D target;                      // 0x48
    f32 reach;                             // 0x54
    u8 pad_0x58;
    bool reached;        // 0x59
    bool field_0x5a;
    bool waiting;        // 0x5b
    bool field_0x5c;
    u8 pad_0x5d;
    bool field_0x5e;
    u8 pad_0x5f;
    f32 timer;           // 0x60
    f32 delay;           // 0x64
    bool field_0x68;
};

void fn_71005c8160(UnkInfesterFleeFromTargetAction_l5c885c* self, bool flag);

static inline UnkInfesterMovement_l5c885c* Movement(UnkInfesterActor_l5c885c* actor) {
    UnkReflObject* movement = actor->movement;
    if (movement && fn_710009d50c(movement->vf_0x0(), fn_71010c74e0()))
        return static_cast<UnkInfesterMovement_l5c885c*>(movement);
    return nullptr;
}

// Whether the reactor's current state is a kind 5 one with something set.
static inline bool IsReacting(UnkInfesterReactor_l5c885c* reactor) {
    UnkInfesterState_l5c885c* state = reactor->state;
    return state->node && state->node->leaf && state->node->leaf->kind == 5 && state->field_0x28;
}

static inline bool CanTurn(UnkInfesterActor_l5c885c* actor) {
    return actor && actor->animation && !fn_710117b348(actor->animation, fn_7100d2b3a8());
}

// A kind 5 parameter block.
static inline void InitKind5(UnkParams* params) {
    __builtin_memset(params->field_0x8, 0, 13);
    params->field_0x30 = 0;
    __builtin_memset(params->field_0x20, 0, 16);
    params->vtable = g_unk_7101a25b10 + 2;
    params->field_0x39 = 0;
    params->field_0x3b = 0;
    params->field_0x3d = 0;
    params->field_0x34 = 5;
    params->field_0x18 = 0;
    params->field_0x1c = 1;
}

static inline bool IsInfesterReady(UnkInfesterAI_l5c885c* ai) {
    return ai && fn_710009d50c(ai->vf_0x0(), fn_71005ca678()) && !ai->field_0x146dd;
}

u32 fn_71005c885c(UnkInfesterFleeFromTargetAction_l5c885c* self, f32 dt) {
    if (self->waiting) {
        void* x = self->actor->field_0x218;
        if (x && fn_71010cc3a4(x) == 4)
            return 2;
        self->waiting = false;
    }

    const UnkPolyline108_b67a1* path = reinterpret_cast<const UnkPolyline108_b67a1*>(self->ai->path);
    if (fn_710035e53c(path)) {
        self->field_0x68 = fn_7100118be8(path);
        if (self->field_0x5e) {
            if (!self->ai->field_0x1220) {
                self->timer += dt;
                if (!(self->timer >= self->delay))
                    return 2;
                if (IsReacting(self->reactor))
                    return 2;
                if (!CanTurn(self->actor))
                    return 2;
                if (fn_71010cc3a4(Movement(self->actor)) == 4)
                    return 2;
                if (!IsInfesterReady(self->ai))
                    return 2;
                UnkParams params;
        InitKind5(&params);
        fn_71006f4b0c(self->reactor, &params, 1);
                base::reflection::refl_EmitU32(self->actor->animation, 0xe2ea001660c8ac66, 0xf4061ff25a2163d6, 2,
                                               &g_unk_710164b958, &g_unk_7172769740);
                return 2;
            }
            if (fn_71010cc3a4(Movement(self->actor)) != 4 && self->field_0x68)
                fn_71005c8160(self, false);
            self->timer = 0.0f;
            return 2;
        }
    } else {
        self->field_0x5e = false;
        self->field_0x68 = true;
    }

    self->reached = false;
    UnkInfesterMovement_l5c885c* movement = Movement(self->actor);
    if (fn_71010cc3a4(movement) != 4)
        fn_71005c8160(self, true);
    if (self->field_0x5a)
        self->reached = true;
    self->bounds = fn_71010cc3a4(movement) == 1 ? &movement->field_0x8e8 : nullptr;
    if (!self->bounds)
        return 2;

    {
        // The bounds' point is within the reach of the target on both axes.
        CVector2D p;
        fn_710007b430(&p, &self->target);
        f32 r = self->reach;
        if (p.x + r >= self->bounds->x && p.x - r <= self->bounds->x && p.y + r >= self->bounds->y &&
            p.y - r <= self->bounds->y)
            self->reached = true;
    }
    if (self->reached && !IsReacting(self->reactor) && IsInfesterReady(self->ai) && CanTurn(self->actor)) {
        UnkParams params;
        InitKind5(&params);
        fn_71006f4b0c(self->reactor, &params, 1);
        base::reflection::refl_EmitU32(self->actor->animation, 0xe2ea001660c8ac66, 0xf4061ff25a2163d6, 2,
                                       &g_unk_710164b958, &g_unk_7172769740);
    }
    if (self->field_0x5c && self->reached)
        return 5;
    return 2;
}
