// CAbilitySpeedBooster's unit (tools/tu_map.py): the ability's per-frame check. Switches the boost state
// machine (+0x3a8) to state 3 ("none" is 4) when the owner may not keep boosting (its speed-booster track,
// unless the tunable's +0x38 says otherwise, or the boost key, under the game manager's slow-motion flags;
// the player's speed against the Samus movement's), else keeps the owner's boost animation (+0x3a2) in step
// with fn_71010d1598. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CMovementComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkAnimComponent_b52;
void fn_7101178e08(UnkAnimComponent_b52* self, u32 value, bool apply, bool flag, f32 time);
struct UnkActorTarget;
void fn_710117662c(UnkActorTarget*, u32 value, u32 extra, bool apply, bool flag, f32 time);
extern base::reflection::UnkTunable g_unk_71726b0fc0;

struct UnkBoostState_l33dec8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);  // entered
    virtual void vf_0x18(s32 next);      // left
    u64 field_0x8;
    s32 id;
};

struct UnkBoostMachine_l33dec8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(UnkBoostState_l33dec8* previous, UnkBoostState_l33dec8* next);
    virtual void vf_0x28(UnkBoostState_l33dec8* current, UnkBoostState_l33dec8* next);

    u64 field_0x8;
    UnkBoostState_l33dec8* current;
    UnkBoostState_l33dec8* previous;
    u32 count;
    u8 storage[16 * sizeof(UnkBoostState_l33dec8*)];
    s32 pending;
    bool busy;

    UnkBoostState_l33dec8** begin() { return reinterpret_cast<UnkBoostState_l33dec8**>(storage); }
    UnkBoostState_l33dec8** end() { return begin() + count; }

    UnkBoostState_l33dec8* Find(s32 id) {
        UnkBoostState_l33dec8** it = begin();
        for (; it != end(); ++it) {
            if ((*it)->id == id)
                break;
        }
        return it != end() ? *it : nullptr;
    }

    void Request(s32 id) {
        pending = id;
        if (busy)
            return;
        if (!current || current->id != id) {
            if (UnkBoostState_l33dec8* next = Find(id)) {
                if (current) {
                    current->vf_0x18(id);
                    vf_0x28(current, next);
                }
                previous = current;
                current = next;
                next->vf_0x10(previous ? previous->id : 4);
                vf_0x20(previous, next);
            }
        }
        pending = 4;
    }
};

struct UnkBoostSamusMovement_l33dec8 {
    u8 pad_0x0[0xc0];
    f32 max_speed;  // 0xc0
};

struct UnkBoostPlayer_l33dec8 {
    u8 pad_0x0[0x10];
    UnkReflObject* movement;  // 0x10
    u8 pad_0x18[0xf9 - 0x18];
    bool limited;  // 0xf9
    u8 pad_0xfa[0x200 - 0xfa];
    f32 speed;  // 0x200
};

struct UnkBoostOther_l33dec8 {
    u8 pad_0x0[0x7c];
    f32 field_0x7c;
};

struct UnkBoostActor_l33dec8 {
    u8 pad_0x0[0x208];
    UnkBoostPlayer_l33dec8* player;  // 0x208
    UnkBoostOther_l33dec8* field_0x210;
    u8 pad_0x218[0x8];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkBoostOwner_l33dec8 {
    u8 pad_0x0[0x30];
    UnkBoostActor_l33dec8* actor;  // 0x30
};

struct UnkSpeedBooster_l33dec8 {
    u8 pad_0x0[0xc0];
    UnkBoostOwner_l33dec8* owner;  // 0xc0
    u8 pad_0xc8[0x3a2 - 0xc8];
    bool animating;  // 0x3a2
    u8 pad_0x3a3[0x3a8 - 0x3a3];
    UnkBoostMachine_l33dec8 machine;  // 0x3a8
};
static_assert(__builtin_offsetof(UnkSpeedBooster_l33dec8, machine.pending) == 0x44c, "layout");

// CAbilitySpeedBooster's tunable.
struct UnkSpeedBoosterTunable_l33dec8 {
    u8 pad_0x0[0x38];
    bool field_0x38;
};

struct UnkIdHolder_l33dec8 {
    u8 pad_0x0[0x30];
    u64 hash;
};

static inline const UnkSpeedBoosterTunable_l33dec8& Tunable() {
    return reinterpret_cast<const UnkSpeedBoosterTunable_l33dec8&>(g_unk_71726b0fc0);
}

static inline bool ManagerAllows() {
    UnkGameManager* manager = g_unk_7101d47310;
    return (manager->field_0xc8 != 0 || manager->field_0x10c) && manager->field_0xc4 && !manager->field_0xc6;
}

static inline void UpdateAnimation(UnkSpeedBooster_l33dec8* self) {
    UnkBoostActor_l33dec8* actor = self->owner->actor;
    bool on = false;
    if (UnkBoostPlayer_l33dec8* player = actor->player) {
        UnkAnimationComponent* animation = actor->animation;
        on = fn_71010d1598(reinterpret_cast<UnkGroundPlayer_b52*>(player));
        if (self->animating != on) {
            if (on)
                fn_710117662c(reinterpret_cast<UnkActorTarget*>(animation), 3, 1, true, true, -1.0f);
            else
                fn_7101178e08(reinterpret_cast<UnkAnimComponent_b52*>(animation), 3, true, true, -1.0f);
        }
    }
    self->animating = on;
}

void fn_710033dec8(UnkSpeedBooster_l33dec8* self) {
    UnkBoostActor_l33dec8* actor = self->owner->actor;
    if (!actor)
        return;
    UnkBoostPlayer_l33dec8* player = actor->player;
    if (!player) {
        if (UnkBoostOther_l33dec8* other = actor->field_0x210) {
            if (other->field_0x7c == 0.0f || ManagerAllows())
                self->machine.Request(3);
        }
        return;
    }
    UnkReflObject* movement = player->movement;
    auto* samus = movement && fn_710009d50c(movement->vf_0x0(), fn_7100795584())
                      ? reinterpret_cast<UnkBoostSamusMovement_l33dec8*>(movement)
                      : nullptr;
    UnkAnimationComponent* animation = actor->animation;
    if (!animation)
        return;
    bool restricted = fn_710117b348(animation, fn_7100d268c8());
    if (restricted && !Tunable().field_0x38) {
        u64 key = reinterpret_cast<UnkIdHolder_l33dec8*>(GameManagerIdTable().field_0x1e50.data)->hash;
        restricted = fn_710117ae38(animation, key, false);
    }
    bool allowed = !restricted || ManagerAllows();
    if (allowed && (!samus || !player->limited || player->speed > samus->max_speed)) {
        self->machine.Request(3);
        return;
    }
    UpdateAnimation(self);
}
