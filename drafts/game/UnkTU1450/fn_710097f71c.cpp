// Unit 1450 (tools/tu_map.py): a movement state's parameter step. It resets the target's matrix, turns its
// transform a quarter turn towards the side its matrix faces, then builds a movement request: while the
// movement is still moving, one that pushes on at up to twice its speed (its track by the owner's
// orientation and the step's mode), else, when the movement allows it, another whose track depends on the
// owner's animation. The request is stored in the reflected value the caller passed. Names and layouts are
// placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU1730.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_l97f71c {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h, flags and a mask.
struct UnkMoveTransition_l97f71c {
    s64 id;
    UnkDesc16_l97f71c desc;
    UnkTrackBlend blend;
    u32 flags;
    u32 mask;

    UnkMoveTransition_l97f71c(UnkDesc16_l97f71c d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), flags(0), mask(m) {}
    UnkMoveTransition_l97f71c()
        : UnkMoveTransition_l97f71c(*reinterpret_cast<const UnkDesc16_l97f71c*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_l97f71c) == 0x30, "layout");

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8), given the vtable of the state it asks for.
struct UnkStateParams_l97f71c {
    void* vtable;
    UnkMoveTransition_l97f71c transition;  // 0x8
    u32 field_0x38;
    UnkVec3f push;  // 0x3c

    explicit UnkStateParams_l97f71c(void* state_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        push.x = 0.0f;
        push.y = 0.0f;
        push.z = 0.0f;
        vtable = state_vtable;
    }
    // The reflected class (the first virtual function).
    const CType* GetClass() { return (*reinterpret_cast<const CType* (**)(UnkStateParams_l97f71c*)>(vtable))(this); }
};
static_assert(sizeof(UnkStateParams_l97f71c) == 0x48, "layout");

struct UnkOwnerState_l97f71c {
    u8 pad_0x0[0x360];
    bool field_0x360;
};

struct UnkOwner_l97f71c {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkStruct_710117af10* animation;  // 0x220
    u8 pad_0x228[0x2b0 - 0x228];
    UnkOwnerState_l97f71c* state;  // 0x2b0
};
static_assert(__builtin_offsetof(UnkOwner_l97f71c, state) == 0x2b0, "layout");

struct UnkMovement_l97f71c {
    u8 pad_0x0[0x30];
    UnkOwner_l97f71c* owner;  // 0x30
    u8 pad_0x38[0xf9 - 0x38];
    bool moving;  // 0xf9
    u8 pad_0xfa[0x1d8 - 0xfa];
    UnkVec3f velocity;  // 0x1d8
};
static_assert(__builtin_offsetof(UnkMovement_l97f71c, velocity) == 0x1d8, "layout");

struct UnkMoveContext_l97f71c {
    u8 pad_0x0[0x38];
    UnkMovement_l97f71c* movement;  // 0x38
};

struct UnkMoveHolder_l97f71c {
    u8 pad_0x0[0x8];
    UnkMoveContext_l97f71c* context;  // 0x8
};

struct UnkMoveStep_l97f71c {
    u8 pad_0x0[0x8];
    UnkMoveHolder_l97f71c* holder;  // 0x8
    s32 mode;                       // 0x10
};

// What the step writes: a matrix and a transform.
struct UnkMoveTarget_l97f71c {
    u8 pad_0x0[0x78];
    UnkMatrix44 matrix;  // 0x78
    UnkXform xform;      // 0xb8
};
static_assert(__builtin_offsetof(UnkMoveTarget_l97f71c, xform) == 0xb8, "layout");

struct UnkUnit1452_b56;

// The id table's ids at offsets UnkGameManagerIdTable.h does not name yet.
static inline const CStrId& IdAt(u32 offset) {
    return *reinterpret_cast<const CStrId*>(reinterpret_cast<const char*>(&GameManagerIdTable()) + offset);
}

static inline s32 Sign(f32 v) {
    return (v > 0.0f) - (v < 0.0f);
}

// A float marked as set (the float's bits in the low word, the flag in the high).
static inline u64 SetValue(f32 v) {
    u32 bits;
    __builtin_memcpy(&bits, &v, sizeof(bits));
    return (u64)1 << 32 | bits;
}

// Stores the request with its dynamic type in the reflected value: in place when the value already holds a
// compatible type, else by resetting and rebuilding it.
static inline void Store(UnkReflValue* value, UnkStateParams_l97f71c* params) {
    const CType* t = params->GetClass();
    const CType* cur = reinterpret_cast<const CType*>(value->type);
    if (cur && value->field_0x14 == 0 &&
        (cur == t ? *reinterpret_cast<void* const*>(reinterpret_cast<const char*>(t) + 0x48) != nullptr
                  : fn_710009d52c(t, cur))) {
        fn_71000a1cc8(value, t, params);
    } else {
        fn_71000a1c60(value);
        fn_71000a18d4(value, t, params, false);
    }
}

void fn_710097f71c(UnkUnit1452_b56* object, void* arg, UnkReflValue* value) {
    UnkMoveStep_l97f71c* self = reinterpret_cast<UnkMoveStep_l97f71c*>(object);
    UnkMoveTarget_l97f71c* target = static_cast<UnkMoveTarget_l97f71c*>(arg);
    UnkMovement_l97f71c* movement = self->holder->context->movement;
    UnkOwner_l97f71c* owner = movement->owner;
    UnkStruct_710117af10* animation = owner->animation;
    UnkOwnerState_l97f71c* state = owner->state;

    UnkMatrix44& m = target->matrix;
    for (s32 i = 0; i < 16; i++)
        m.m[i] = 0.0f;
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    f32 yaw = Sign(target->xform.GetMatrix().m[2]) * 1.5707964f;
    target->xform.SetAngle(UnkVec3f(0.0f, yaw, 0.0f));

    if (movement->moving) {
        UnkStateParams_l97f71c params(g_unk_7101a22ce8 + 0x10);
        params.transition.flags |= 1;
        const UnkMatrix44& orientation = owner->xform.GetMatrix();
        if (self->mode == 2) {
            bool locked = state->field_0x360;
            if (orientation.m[5] > 0.98f && !locked) {
                params.transition.id = IdAt(0x1228).data->Key();
                params.transition.flags |= 2;
            } else {
                params.transition.id = IdAt(0xfd0).data->Key();
                params.transition.desc.a = -1;
                params.transition.desc.b = SetValue(0.06f);
            }
        } else {
            params.transition.id = IdAt(0xfd0).data->Key();
            params.transition.flags |= 0x80;
            params.transition.desc = *reinterpret_cast<const UnkDesc16_l97f71c*>(&g_unk_710164b968);
        }
        params.field_0x38 = 1;
        f32 speed = fn_71009e754c(reinterpret_cast<UnkMove_b152*>(movement));
        f32 limit = speed * 2.0f;
        params.push.y = movement->velocity.y;
        f32 vx = movement->velocity.x;
        params.push.z = movement->velocity.z;
        s32 sign = Sign(vx);
        f32 v = __builtin_fabsf(vx);
        params.push.x = (limit < v ? limit : v) * sign;
        params.transition.flags |= 8;
        Store(value, &params);
    } else if (fn_71009e6f9c(reinterpret_cast<UnkHelper*>(movement))) {
        UnkStateParams_l97f71c params(reinterpret_cast<char*>(g_unk_7101a22d18) + 0x10);
        bool alt = CAnimationComponent_IsPlayingActionNameContaining(animation, reinterpret_cast<UnkCls_710117af10**>(const_cast<CStrId*>(&IdAt(0x20a0))),
                                 false) ||
                   CAnimationComponent_IsPlayingActionNameContaining(animation, reinterpret_cast<UnkCls_710117af10**>(const_cast<CStrId*>(&IdAt(0x2518))),
                                 false);
        params.transition.id = (alt ? IdAt(0x1d98) : IdAt(0x1d78)).data->Key();
        params.transition.flags |= 2;
        params.transition.flags |= 8;
        Store(value, &params);
    }
}
