// CQuetzoaAIComponent's unit (tools/tu_map.py unit 973): starts a dodge. Picks the side away from the target
// (falling back to the other one when the first is blocked and the target is close), sends the animation
// the matching dodge, and when it was taken turns the owner to face along its x axis, hooks the link to the
// animation and sets "bIsDodging" on the blackboard. Returns whether the dodge started.
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAbilityShinespark.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "game/UnkTU0.h"
#include "game/UnkTU54.h"
#include "game/UnkTypedValue.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

// Called back through the link while it is hooked to the animation.
void fn_710064bea0();

// A listener link: its owner and the callback.
struct UnkDodgeLink_l64b3f4 {
    u8 pad_0x0[0x8];
    void* owner;
    void (*callback)();
};

// The dodge state: whether the dodge started, how far to check on the near and far side, the link and the
// weak handle on the animation it is hooked to.
struct UnkDodge_l64b3f4 {
    bool started;
    u8 pad_0x1[3];
    f32 near_reach;  // 0x4
    f32 far_reach;   // 0x8
    u8 pad_0xc[0xc];
    UnkDodgeLink_l64b3f4 link;  // 0x18
    UnkWeakHandle* handle;      // 0x30
};

struct UnkQuetzoaAI_l64b3f4 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1228];
    CBlackboard blackboard;  // 0x1260
    u8 pad_0x1260[0x14ae8 - 0x1260 - sizeof(CBlackboard)];
    UnkDodge_l64b3f4 dodge;  // 0x14ae8
};
static_assert(__builtin_offsetof(UnkQuetzoaAI_l64b3f4, dodge) == 0x14ae8, "UnkQuetzoaAI_l64b3f4");

// Whether the way is clear at `offset` from the owner.
bool CQuetzoaAIComponent_SweepCollidesAlongDelta(UnkQuetzoaAI_l64b3f4* self, const CVector3D* offset);

// The blend a dodge is sent with: no id, 0.06 seconds.
struct UnkDodgeBlend_l64b3f4 {
    s64 id;
    f32 time;
    u32 field_0xc;

    UnkDodgeBlend_l64b3f4() : id(-1), time(0.06f), field_0xc(1) {}
};

static inline f32 Sign(f32 x) {
    return (f32)((x > 0.0f) - (x < 0.0f));
}

// The sign of the owner's facing: the x of its matrix's third column.
static inline f32 Facing(UnkActor* owner) {
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    return Sign(m.m[2]);
}

// vtable slot 0xd8: turns the actor to `angles`.
static inline void SetAngles(UnkActor* actor, const UnkVec3f& angles, bool flag) {
    reinterpret_cast<void (*)(UnkActor*, const UnkVec3f*, bool)>(actor->vtable[0xd8 / 8])(actor, &angles, flag);
}

// Sends the animation the dodge under `hash` through the caller's two arguments; whether it was taken.
static inline bool SendDodge(void* animation, u64 hash, base::reflection::UnkReflTypeIdArg& mode,
                             base::reflection::UnkReflTypeIdArg& mask) {
    mode.id = 0;
    UnkDodgeBlend_l64b3f4 blend;
    mask.id = UnkAll_SpitSend().value;
    return fn_7101176690(animation, hash, 0xf4061ff25a2163d6, &mode, &blend, &g_unk_7172769740, &mask);
}

// Hooks the link to the animation: unhooks it from what the handle held, then takes a handle on the animation.
static inline void Hook(UnkQuetzoaAI_l64b3f4* self) {
    UnkDodge_l64b3f4* dodge = &self->dodge;
    dodge->link.owner = self;
    dodge->link.callback = &fn_710064bea0;
    UnkWeakTarget* target = static_cast<UnkWeakTarget*>(self->owner->field_0x220);
    if (UnkWeakHandle* handle = dodge->handle) {
        if (UnkWeakTarget* held = handle->target) {
            fn_710117b3f0(held, &dodge->link);
            handle = dodge->handle;
        }
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
            dodge->handle = nullptr;
        }
    }
    if (target) {
        CAbilityGhostAura_AssignWeakHandleToRef(target, &dodge->handle);
        fn_710117b3e8(dodge->handle ? dodge->handle->target : nullptr, &dodge->link);
    }
}

bool fn_710064b3f4(UnkQuetzoaAI_l64b3f4* self) {
    UnkDodge_l64b3f4* dodge = &self->dodge;
    if (void* animation = self->owner->field_0x220) {
        f32 facing = Facing(self->owner);
        f32 dx = self->owner->field_0x44 - fn_7100d7c990(self->owner)->field_0x44;
        f32 side = Sign(dx);
        CVector3D offset;
        // Declared past a goto (no lifetime markers), so each keeps its own slot.
        goto probe;
        base::reflection::UnkReflTypeIdArg away_mode;
        base::reflection::UnkReflTypeIdArg away_mask;
        base::reflection::UnkReflTypeIdArg toward_mode;
        base::reflection::UnkReflTypeIdArg toward_mask;
    probe:
        offset.x = (side == facing ? dodge->near_reach : dodge->far_reach) * facing;
        offset.y = 0.0f;
        offset.z = 0.0f;
        if (CQuetzoaAIComponent_SweepCollidesAlongDelta(self, &offset)) {
            // Blocked: the other side, when the target is close.
            if (!(__builtin_fabsf(dx) < 200.0f))
                return dodge->started;
            side = -side;
            offset.x = (facing == side ? dodge->near_reach : dodge->far_reach) * facing;
            if (CQuetzoaAIComponent_SweepCollidesAlongDelta(self, &offset))
                return dodge->started;
        }
        {
            if (side == facing)
                dodge->started = SendDodge(animation, 0x809cf8f8ffb0ef84, away_mode, away_mask);
            else
                dodge->started = SendDodge(animation, 0xa8e3580f3384f703, toward_mode, toward_mask);
            if (dodge->started) {
                UnkActor* owner = self->owner;
                SetAngles(owner, fn_710007a7f0(UnkVec3f(Facing(owner), 0.0f, 0.0f)), true);
                Hook(self);

                CStrId name("bIsDodging", true);
                bool value = true;
                if (CBlackboard::CSection* section = fn_71010472e0(&self->blackboard, g_unk_71726b7cb8)) {
                    if (!self->blackboard.field_0xe8) {
                        UnkTypedValue v;
                        v.type = fn_710000d228();
                        v.value = &value;
                        fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&name), &v);
                    }
                }
            }
        }
    }
    return dodge->started;
}
