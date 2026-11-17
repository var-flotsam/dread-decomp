// CSamusGrappleMovementState's unit (tools/tu_map.py, unit 1461): picks the animation action the grapple
// state plays (a pair of action ids, the second for when the aim points back against the facing) and
// returns it as a request with the default time and blend. Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkStateMachineFind.h"
#include "game/UnkTU1730.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkGrappleMoveState_B71a6;
bool fn_71009ad430(UnkGrappleMoveState_B71a6* state);
u32 fn_71009a60fc();

// The time an action starts with (the default is g_unk_710164b958).
struct UnkGrappleActionTime_l99bab0 {
    s64 key;
    f32 time;
    u32 count;
};

// The blend an action uses (the default is g_unk_7172769740).
struct UnkGrappleActionBlend_l99bab0 {
    u32 a, b, c;
};

static inline const UnkGrappleActionTime_l99bab0& DefaultTime() {
    return reinterpret_cast<const UnkGrappleActionTime_l99bab0&>(g_unk_710164b958);
}
static inline const UnkGrappleActionBlend_l99bab0& DefaultBlend() {
    return reinterpret_cast<const UnkGrappleActionBlend_l99bab0&>(g_unk_7172769740);
}

struct UnkGrappleActionRequest_l99bab0 {
    u64 action;
    UnkGrappleActionTime_l99bab0 time;
    UnkGrappleActionBlend_l99bab0 blend;
    u32 mode;
    u32 mask;

    UnkGrappleActionRequest_l99bab0(u64 action_, UnkGrappleActionTime_l99bab0 time_,
                                    UnkGrappleActionBlend_l99bab0 blend_, u32 mode_, u32 mask_)
        : action(action_), time(time_), blend(blend_), mode(mode_), mask(mask_) {}
};

// The record the track query resolves: the action it names and the action played instead when mirrored.
struct UnkGrappleTrackRecord_l99bab0 {
    u8 pad_0x0[0x18];
    base::global::CStrId action;
    u8 pad_0x20[0x18];
    base::global::CStrId mirrored;
};

struct UnkGrappleMoveState_l99bab0 {
    u8 pad_0x0[0x10];
    u32 id;
};

struct UnkGrappleAnimActor_l99bab0 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

#pragma pack(push, 4)
struct UnkGrappleMovement_l99bab0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkGrappleAnimActor_l99bab0* actor;
    u8 pad_0x38[0x20];
    f32 facing;
    u8 pad_0x5c[0xf9 - 0x5c];
    bool field_0xf9;
    u8 pad_0xfa[0x2d18 - 0xfa];
    u32 state_count;
    UnkGrappleMoveState_l99bab0* states[1];  // 0x2d1c, 4-byte aligned

    bool vf_0x3b8() { return reinterpret_cast<bool (*)(UnkGrappleMovement_l99bab0*)>(vtable[0x3b8 / 8])(this); }
};
#pragma pack(pop)
static_assert(__builtin_offsetof(UnkGrappleMovement_l99bab0, states) == 0x2d1c, "layout");

struct UnkGrappleAim_l99bab0 {
    u8 pad_0x0[0x2b8];
    f32 angle;
    u8 pad_0x2bc[0x360 - 0x2bc];
    bool active;
};

struct UnkGrappleOwner_l99bab0 {
    u8 pad_0x0[0x38];
    UnkXform xform;
    u8 pad_0x94[0x2b0 - 0x94];
    UnkGrappleAim_l99bab0* aim;
};
static_assert(__builtin_offsetof(UnkGrappleOwner_l99bab0, aim) == 0x2b0, "layout");

struct UnkGrappleState_l99bab0 {
    u8 pad_0x0[0x30];
    UnkGrappleOwner_l99bab0* owner;
    UnkGrappleMovement_l99bab0* movement;
    u8 pad_0x40[0xbac - 0x40];
    bool field_0xbac;
    bool field_0xbad;
    bool field_0xbae;
    bool field_0xbaf;
    bool field_0xbb0;
    bool field_0xbb1;
};

static inline UnkGrappleMoveState_l99bab0* FindState(UnkGrappleMovement_l99bab0* self, u32 kind) {
    if (kind > 14)
        return nullptr;
    return FindStateById(self->states, self->state_count, kind);
}

static inline bool IsPlaying(UnkGrappleAnimActor_l99bab0* actor, const base::reflection::CClass* cls) {
    return fn_710117b348(actor->animation, cls);
}

// Aiming back over the shoulder: the aim points behind and the facing is not the side the owner turns to.
static inline bool IsAimingBack(UnkGrappleState_l99bab0* self) {
    UnkGrappleOwner_l99bab0* owner = self->owner;
    UnkGrappleAim_l99bab0* aim = owner->aim;
    if (!aim->active || !(aim->angle > 1.5707964f && aim->angle < 4.712389f))
        return false;
    f32 facing = self->movement->facing;
    f32 y = owner->xform.GetAngle().y;
    return facing != (f32)((y > 0.0f) - (y < 0.0f));
}

UnkGrappleActionRequest_l99bab0 fn_710099bab0(UnkGrappleState_l99bab0* self) {
    UnkGrappleActionRequest_l99bab0 request(-1, DefaultTime(), DefaultBlend(), 0x91, UnkAll_SpitSend().value);
    UnkGrappleAnimActor_l99bab0* actor = self->movement->actor;
    UnkTrackQuery query;
    query.cls = fn_7100c59c3c();
    query.result = nullptr;
    query.value = -1.0f;
    fn_710117b08c(actor->animation, &query);

    u64 action;
    u64 mirrored;
    bool stopping = false;
    auto* record = reinterpret_cast<UnkGrappleTrackRecord_l99bab0*>(query.result);
    if (record && record->action.data != &g_unk_7101d47108->empty &&
        (action = record->action.data->Key()) != (u64)-1) {
        mirrored = record->mirrored.data->Key();
    } else if (self->field_0xbad || self->field_0xbae) {
        UnkGrappleMovement_l99bab0* movement = self->movement;
        UnkGrappleMoveState_l99bab0* state = FindState(movement, fn_71009a60fc());
        if (fn_71009ad430(reinterpret_cast<UnkGrappleMoveState_B71a6*>(state))) {
            action = 0x7964b69ce0e76726;
            mirrored = 0x9c234a72775b8845;
        } else {
            action = 0x92ea8dfb023344fd;
            mirrored = 0xca87256dd06096e7;
        }
    } else if (self->field_0xbac) {
        if (self->movement->vf_0x3b8()) {
            stopping = true;
        } else {
            action = 0x5e77f22362f4ca41;
            mirrored = 0x4a00cd44f41b7670;
        }
    } else if (self->field_0xbaf) {
        action = self->field_0xbb0 ? 0xd08f2832f76c457f : 0xf051cc33348493b5;
        mirrored = 0xc6e6978254cc71b2;
    } else if (IsPlaying(actor, fn_7100d25ea8()) || IsPlaying(actor, fn_7100d2b3a8()) ||
               IsPlaying(actor, fn_7100d2ae9c()) || self->movement->field_0xf9) {
        stopping = true;
    } else if (self->field_0xbb1) {
        action = 0xbd6f7fef3c42980a;
        mirrored = 0x383c794781d99104;
    } else {
        action = 0x5bd175c6418d84e3;
        mirrored = 0xc6e6978254cc71b2;
    }
    if (stopping) {
        request.time.key = -1;
        request.time.time = 0.12f;
        request.time.count = 1;
        action = 0xa51fb6e11deb3101;
        mirrored = 0x57298d372d25303f;
    }

    if (IsAimingBack(self))
        request.action = mirrored;
    else
        request.action = action;
    return request;
}
