// CShelmitAIComponent's unit (tools/tu_map.py unit 1054). The message handler (fn_71006bb668): after the base
// AI handler (fn_710036974c), whose result it returns, a flagged message in mode 0 (+0x14780) restarts the
// timer pair (+0x147a0), runs fn_71006ba61c and, when vf_0x490 allows, tells the held target's +0x208 object
// to stop and drops the target; an unflagged one calls fn_71006ba4fc once +0x147a8 reaches about 0.999. The
// grab update (fn_71006ba7ec): with a held target and tuning (+0x146c8), counts time at +0x14790 up to the
// tuning's limit (then fn_71006baa5c), otherwise writes the progress of the other actor's grapple track into
// the owner animation's property bag; then counts the cooldown at +0x14798 down. The blend
// (fn_71006ba90c): a value between the tuning's two values mapped from the owner's horizontal speed in the
// tuning's speed range and signed by its facing (0 while the movement at +0x1f0 is stopped), not below 0,
// written to slot 0 of the shape's slot table. The parameter copy (fn_71006bc2a0): the tuning's parameter
// set at +0x648 while the owner's state (+0x1d0) has both low flag bits and the mode is not 1. Names and
// layouts are placeholders.
#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CBehaviorTreeAIComponent.h"
#include "game/CShelmitAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkSlots.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2523.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSclawkAI_b44a2;
struct UnkSclawkMessage_b44a2;
struct UnkShelmitAI_ovb0111;
void fn_71006ba61c(UnkShelmitAI_ovb0111* self);

struct UnkShelmitTarget_ovb0111 {
    u8 pad_0x0[0x208];
    void* field_0x208;
};

struct UnkShelmitOwner_ovb0111 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkShelmitMessage_ovb0111 {
    u8 pad_0x0[0x8];
    bool field_0x8;
};

// The block at +0x14780: a mode, the weak handle of a target and some timers.
struct UnkShelmitBlock_ovb0111 {
    s32 mode;
    u8 pad_0x4[0x4];
    UnkWeakHandle* handle;
    u32 field_0x10;
    u8 pad_0x14[0x8];
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    f32 field_0x28;
};

struct UnkShelmitAI_ovb0111 {
    struct Vtable {
        u8 pad_0x0[0x490];
        bool (*vf_0x490)(UnkShelmitAI_ovb0111* self);
    }* vtable;
    u8 pad_0x8[0x28];
    UnkShelmitOwner_ovb0111* owner;
    u8 pad_0x38[0x14780 - 0x38];
    UnkShelmitBlock_ovb0111 block;
};

u32 fn_71006bb668(UnkShelmitAI_ovb0111* self, UnkShelmitMessage_ovb0111* msg) {
    UnkShelmitBlock_ovb0111* block = &self->block;
    u32 result = fn_710036974c(reinterpret_cast<UnkSclawkAI_b44a2*>(self), reinterpret_cast<UnkSclawkMessage_b44a2*>(msg));
    if (msg->field_0x8) {
        if (block->mode == 0) {
            block->field_0x20 = block->field_0x1c;
            block->field_0x24 = 0;
            fn_71006ba61c(self);
            if (self->vtable->vf_0x490(self)) {
                if (self->owner->field_0x220) {
                    if (UnkWeakHandle* handle = block->handle) {
                        if (UnkShelmitTarget_ovb0111* target =
                                reinterpret_cast<UnkShelmitTarget_ovb0111*>(handle->target)) {
                            if (target->field_0x208)
                                fn_71009e85f4(target->field_0x208, true);
                        }
                    }
                }
                block->field_0x10 = 0;
                ResetRef(&block->handle);
            }
        }
    } else if (block->field_0x28 >= 0x1.ff7ced91p-1 /* about 0.999 */) {
        fn_71006ba4fc(self);
    }
    return result;
}

extern char g_unk_71727071a8[] __attribute__((visibility("hidden")));  // the unit's property id

struct UnkShelmitGrabTarget_ovb0111 {
    u8 pad_0x0[0x61c];
    f32 limit;  // 0x61c
};

struct UnkShelmitGrabState_ovb0111 {
    UnkShelmitGrabTarget_ovb0111* target;  // 0x0
    u8 pad_0x8[0xb8];
    UnkWeakHandle* handle;  // 0xc0
    f32 value;  // 0xc8
    u8 pad_0xcc[0x4];
    f32 cooldown;  // 0xd0
};

// A grapple track's record: its range and how the value maps into it.
struct UnkShelmitTrackRecord_ovb0111 {
    u8 pad_0x0[0x10];
    f32 start;
    f32 end;
    s32 kind;
};

struct UnkShelmitGrabAI_ovb0111 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    UnkShelmitGrabState_ovb0111 state;  // 0x146c8
};

struct UnkShelmitAI_b49a3;

void fn_71006ba7ec(UnkShelmitGrabAI_ovb0111* self, f32 dt) {
    UnkShelmitGrabState_ovb0111* state = &self->state;
    if (self->state.handle && self->state.handle->target && state->target) {
        state->value += dt;
        if (state->value >= state->target->limit) {
            fn_71006baa5c(reinterpret_cast<UnkShelmitAI_b49a3*>(self), 0, 0);
        } else if (UnkActor* owner = self->owner) {
            auto* animation = static_cast<UnkAnimationComponent*>(owner->field_0x220);
            UnkActor* other = fn_7100d7c990(owner);
            auto* other_animation = other ? static_cast<UnkAnimationComponent*>(other->field_0x220) : nullptr;
            if (animation && other_animation) {
                UnkTrackQuery query = {fn_7100d001f8(), nullptr, -1.0f};
                fn_710117b08c(other_animation, &query);
                if (auto* record = reinterpret_cast<UnkShelmitTrackRecord_ovb0111*>(query.result)) {
                    f32 v = record->kind == 1 ? query.value / (record->end - record->start) : 0.0f;
                    unk_prop_bag::fn_710033187c(reinterpret_cast<unk_prop_bag::UnkPropBag*>(animation->field_0x80),
                                                *reinterpret_cast<const unk_prop_bag::CStrId*>(g_unk_71727071a8), v);
                }
            }
        }
    }
    if (state->cooldown > 0.001f)
        state->cooldown -= dt;
}

struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkShelmitBlendTarget_ovb0111 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};

struct UnkShelmitBlendShape_ovb0111 {
    u8 pad_0x0[0x50];
    UnkShelmitBlendTarget_ovb0111* target;
};

struct UnkShelmitBlendMovement_ovb0111 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

// The tuning the blend reads: a value range mapped from a speed range.
struct UnkShelmitBlendTuning_ovb0111 {
    u8 pad_0x0[0x624];
    f32 min_value;  // 0x624
    f32 max_value;  // 0x628
    f32 min_speed;  // 0x62c
    f32 max_speed;  // 0x630
};

struct UnkShelmitBlend_ovb0111 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    UnkShelmitBlendTuning_ovb0111* tuning;  // 0x146c8
};

void fn_71006ba90c(UnkShelmitBlend_ovb0111* self) {
    UnkActor* owner = self->owner;
    if (!owner)
        return;
    auto* shape = static_cast<UnkShelmitBlendShape_ovb0111*>(owner->field_0x228);
    if (!shape)
        return;
    UnkShelmitBlendTarget_ovb0111* target = shape->target;
    if (!target)
        return;
    UnkShelmitBlendTuning_ovb0111* tuning = self->tuning;
    if (!tuning)
        return;
    auto* movement = *reinterpret_cast<UnkShelmitBlendMovement_ovb0111**>(owner->pad_0x1e0 + 0x10);  // 0x1f0
    f32 min_value = tuning->min_value;
    f32 value = tuning->max_value;
    f32 min_speed = tuning->min_speed;
    f32 max_speed = tuning->max_speed;
    if (movement && movement->field_0xf9) {
        value = 0.0f;
    } else if (min_value > -0.001f && min_value < value && min_speed > -0.001f && min_speed < max_speed) {
        UnkKraidCreatureVec velocity = fn_7100d7be08(owner);
        f32 speed = __builtin_fabsf(velocity.x);
        f32 clamped = speed <= min_speed ? min_speed : (speed >= max_speed ? max_speed : speed);
        f32 t = (clamped - min_speed) / (max_speed - min_speed);
        value = min_value + (value - min_value) * t;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38));
        f32 facing = m.m[2];
        value *= (f32)((facing > 0.0f) - (facing < 0.0f));
    }
    base::math::CVector4D v;
    v.x = value <= 0.0f ? 0.0f : value;
    v.y = 0.0f;
    v.z = 0.0f;
    v.w = 0.0f;
    fn_71001c2764(target->slots, 0, 0, &v, 1, true);
}

using base::global::CStrId;

// A parameter set the tuning holds and hands out by copy.
struct UnkShelmitParams_ovb0111 {
    u8 head[0x5c];
    CStrId id;  // 0x60
    UnkVec3f a;  // 0x68
    UnkVec3f b;  // 0x74
    u8 tail[11];  // 0x80
};
static_assert(__builtin_offsetof(UnkShelmitParams_ovb0111, tail) == 0x80, "layout");

struct UnkShelmitParamsTuning_ovb0111 {
    u8 pad_0x0[0x648];
    UnkShelmitParams_ovb0111 params;  // 0x648
};

struct UnkShelmitParamsOwnerState_ovb0111 {
    u8 pad_0x0[0x248];
    u8 flags;  // 0x248
};

struct UnkShelmitParamsAI_ovb0111 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    struct Block {
        UnkShelmitParamsTuning_ovb0111* tuning;  // 0x0
        u8 pad_0x8[0xb4];
        s32 field_0xbc;
    } block;  // 0x146c8
};

static inline void CopyParams(UnkShelmitParamsAI_ovb0111::Block& block, UnkActor* owner,
                              UnkShelmitParams_ovb0111* out) {
    UnkShelmitParamsTuning_ovb0111* tuning = block.tuning;
    if (!tuning)
        return;
    auto* state = *reinterpret_cast<UnkShelmitParamsOwnerState_ovb0111**>(owner->pad_0x1c0 + 0x10);  // 0x1d0
    if ((state == nullptr) | (block.field_0xbc == 1))
        return;
    if ((~state->flags & 3) != 0)
        return;
    *out = tuning->params;
}

void fn_71006bc2a0(UnkShelmitParamsAI_ovb0111* self, UnkShelmitParams_ovb0111* out) {
    CopyParams(self->block, self->owner, out);
}
