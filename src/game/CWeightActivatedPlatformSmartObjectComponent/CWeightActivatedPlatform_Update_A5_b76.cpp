// CWeightActivatedPlatformSmartObjectComponent unit (tools/tu_map.py, unit 1516): the update of another state
// (fn_7100a0af50's sibling). When fn_7100a04768 fails or the player's ability (fn_7100327d14 in the ability set at
// +0x2e0) is in state 6, resets the timer at +0x14c and, while the animation plays the singleton's first part
// (+0x1db8), blends the colours at +0x160/+0x150 by the squared progress into the slot table and passes the
// progress to fn_7100a09bf8, or runs fn_7100a09cd4 once it plays the second part (+0x1dc0). Otherwise the timer
// runs and state 2 is requested from the state machine at +0x180 (last 7, "none" 6) once it reaches +0x148.
// fn_7100a0b480 is the counterpart: state 4 when fn_7100a04768 fails or the ability is in state 6; otherwise state 3
// on the first part (+0x1c80) while the player's tilt (+0x208, +0x10c) is level and fn_7100a0ad9c holds, or the
// reverse blend on the second (+0x1c78); then, once the animation reaches fn_7100d0d6d8's class, gives the player
// its input back. Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "base/UnkDeferredSet.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityShinespark.h"
#include "game/CElectricReactionComponent.h"
#include "game/CFXComponent.h"
#include "game/CInputComponent.h"
#include "game/CSmartObjectComponent.h"
#include "game/CWeightActivatedPlatformSmartObjectComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSlots.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;

struct UnkPlatformSm_A5_b72;
struct UnkPlayer;
struct UnkAbilitySet;
struct UnkAbility;
UnkPlayer* fn_7100dec788(UnkGameManager* manager);
s32 fn_7101177da8(void*, u64);
struct UnkWeightPlatform_b49a4;
struct UnkPlatformUpdate_A5_b76;
void fn_7100a09bf8(UnkPlatformUpdate_A5_b76* self, bool a, f32 progress);

bool fn_7100a0ad9c(UnkPlatformUpdate_A5_b76* self);
struct UnkInputComponent;
void fn_710094b508(void* object);
extern CStrId g_unk_7172743570 __attribute__((visibility("hidden")));

struct UnkPlatformSubState_A5_b76 {
    u8 pad_0x0[0x10];
    s32 state;
};
struct UnkPlatformAbility_A5_b76 {
    u8 pad_0x0[0x20];
    UnkPlatformSubState_A5_b76* sub;
    u8 pad_0x28[0xb4 - 0x28];
    s32 state;
};
struct UnkPlatformTilt_A5_b76 {
    u8 pad_0x0[0x10c];
    f32 value;
};
struct UnkPlatformPlayer_A5_b76 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;
    u8 pad_0x1e0[0x208 - 0x1e0];
    UnkPlatformTilt_A5_b76* tilt;
    void* field_0x210;
    u8 pad_0x218[0x2e0 - 0x218];
    UnkAbilitySet* abilities;
};

struct UnkPlatformSlotsOwner_A5_b76 {
    u8 pad_0x0[0x138];
    UnkSlots* slots;
};
struct UnkPlatformSlotsComp_A5_b76 {
    u8 pad_0x0[0x50];
    UnkPlatformSlotsOwner_A5_b76* owner;
};
struct UnkPlatformEntity_A5_b76 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
    UnkPlatformSlotsComp_A5_b76* slots;
};

struct UnkPlatformUpdate_A5_b76 {
    u8 pad_0x0[0x30];
    UnkPlatformEntity_A5_b76* entity;
    u8 pad_0x38[0x148 - 0x38];
    f32 limit;
    f32 timer;
    base::color::CColor4F to;
    base::color::CColor4F from;
    u32 slot;
    u8 pad_0x174[0x180 - 0x174];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0x230[0x5a0 - 0x230];
    u64 track;
};
static_assert(__builtin_offsetof(UnkPlatformUpdate_A5_b76, track) == 0x5a0, "layout");

struct UnkSingletonKey_A5_b76 {
    u8 pad_0x0[0x30];
    u64 hash;
};

static inline UnkSingletonKey_A5_b76* SingletonKey_A5_b76(u32 offset) {
    return *reinterpret_cast<UnkSingletonKey_A5_b76**>(
        reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + offset);
}

static inline bool PlayerHolds_A5_b76() {
    auto* player = reinterpret_cast<UnkPlatformPlayer_A5_b76*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->abilities)
        return false;
    auto* ability =
        reinterpret_cast<UnkPlatformAbility_A5_b76*>(fn_71003013ec(player->abilities, fn_7100327d14()));
    if (!ability)
        return false;
    s32 state = ability->state;
    if (state == 7) {
        if (!ability->sub)
            return false;
        state = ability->sub->state;
    }
    return state == 6;
}

void fn_7100a0bee0(UnkPlatformUpdate_A5_b76* self, f32 dt) {
    if (!fn_7100a04768(reinterpret_cast<UnkPlatformSm_A5_b72*>(self)) || PlayerHolds_A5_b76()) {
        self->timer = 0.0f;
        UnkAnimationComponent* animation = self->entity->animation;
        if (fn_710117ae38(animation, SingletonKey_A5_b76(0x1db8)->hash, false)) {
            f32 done = fn_7101177ea0(animation, self->track);
            s32 total = fn_7101177da8(animation, self->track);
            f32 progress = done / (f32)total;
            UnkPlatformSlotsOwner_A5_b76* owner = self->entity->slots->owner;
            u32 slot = self->slot;
            base::color::CColor4F color = fn_710003088c(progress * progress, self->from, self->to);
            fn_71001c2764(owner->slots, slot, 1, reinterpret_cast<const base::math::CVector4D*>(&color), 1, true);
            fn_7100a09bf8(self, false, progress);
        } else if (fn_710117ae38(animation, SingletonKey_A5_b76(0x1dc0)->hash, false)) {
            fn_7100a09cd4(reinterpret_cast<UnkWeightPlatform_b49a4*>(self));
        }
        return;
    }
    if (self->limit <= 0.0f) {
        RequestState_A5_b73<7, 6>(&self->machine, 2);
        return;
    }
    self->timer += dt;
    if (self->timer >= self->limit)
        RequestState_A5_b73<7, 6>(&self->machine, 2);
}

void fn_7100a0b480(UnkPlatformUpdate_A5_b76* self) {
    auto* player = reinterpret_cast<UnkPlatformPlayer_A5_b76*>(fn_7100dec788(g_unk_7101d47310));
    if (!fn_7100a04768(reinterpret_cast<UnkPlatformSm_A5_b72*>(self)) || PlayerHolds_A5_b76()) {
        RequestState_A5_b73<7, 6>(&self->machine, 4);
    } else {
        UnkAnimationComponent* animation = self->entity->animation;
        if (fn_710117ae38(animation, SingletonKey_A5_b76(0x1c80)->hash, false)) {
            if (player && player->tilt) {
                f32 tilt = player->tilt->value;
                if (!(tilt + 0.001f >= 0.0f) || !(tilt - 0.001f <= 0.0f))
                    goto done;
            }
            if (fn_7100a0ad9c(self))
                RequestState_A5_b73<7, 6>(&self->machine, 3);
        } else if (fn_710117ae38(animation, SingletonKey_A5_b76(0x1c78)->hash, false)) {
            f32 done = fn_7101177ea0(animation, self->track);
            s32 total = fn_7101177da8(animation, self->track);
            f32 progress = done / (f32)total;
            UnkPlatformSlotsOwner_A5_b76* owner = self->entity->slots->owner;
            u32 slot = self->slot;
            base::color::CColor4F color = fn_710003088c(progress * progress, self->to, self->from);
            fn_71001c2764(owner->slots, slot, 1, reinterpret_cast<const base::math::CVector4D*>(&color), 1, true);
            fn_7100a09bf8(self, false, 1.0f - progress);
        }
    }
done:
    if (UnkAnimationComponent* animation = self->entity->animation) {
        bool reached = fn_710117b348(animation, fn_7100d0d6d8());
        if (player && reached) {
            fn_7100872460(player->input, true, false, &g_unk_7172743570);
            if (player->field_0x210)
                fn_710094b508(player->field_0x210);
        }
    }
}

// fn_7100a09948, the platform's own update: tells its entity it is a platform (vf_0x140 with the game manager's
// +0x968), sets the "on" timeline key once (+0x5a8), asks the linked smart object's user for state 3 and, when the
// user is not busy (+0xf8), resets the slot colour (+0x174) and starts the object's track (+0x178); in state 6 (or
// none) it sets the "off" key and runs fn_7100a09cd4. Then it releases the entity's part at +0x2a0 and registers
// the watch at +0x540 with the game manager's set at +0x1da8.
struct UnkSmartObjectUserA;
struct UnkSmartObjectActor;
void fn_7100861fc8(void* part, bool flag);
bool fn_71001c287c(void*, u32, u32, CVector4D*, u32);
struct UnkRinkaControl_ruc;
extern const u64 g_unk_710164b4c0;
extern const u64 g_unk_710164b4c8;

struct UnkPlatformSmartUser_A5_b76 {
    virtual void vf_0x0();
    bool vf_0x180() {
        return reinterpret_cast<bool (*)(UnkPlatformSmartUser_A5_b76*)>((*reinterpret_cast<void***>(this))[0x180 / 8])(
            this);
    }
    u8 pad_0x8[0xf8 - 0x8];
    bool busy;
};

struct UnkPlatformSmartLink_A5_b76 {
    u8 pad_0x0[0x10];
    UnkSmartObjectActor* object;
};

struct UnkPlatformTrackKey_A5_b76 {
    u8 pad_0x0[0x30];
    void* hash;
};

struct UnkPlatformSelfEntity_A5_b76 {
    u8 pad_0x0[0x228];
    UnkPlatformSlotsComp_A5_b76* slots;
    u8 pad_0x230[0x280 - 0x230];
    UnkRinkaControl_ruc* control;
    u8 pad_0x288[0x2a0 - 0x288];
    void* part;
};

struct UnkPlatformSelf_A5_b76 {
    virtual void vf_0x0();
    void vf_0x140(void* arg) {
        reinterpret_cast<void (*)(UnkPlatformSelf_A5_b76*, void*)>((*reinterpret_cast<void***>(this))[0x140 / 8])(this,
                                                                                                                   arg);
    }
    u8 pad_0x8[0x30 - 0x8];
    UnkPlatformSelfEntity_A5_b76* entity;
    u8 pad_0x38[0xb0 - 0x38];
    UnkPlatformSmartLink_A5_b76* link;
    u8 pad_0xb8[0x174 - 0xb8];
    u32 slot;
    UnkPlatformTrackKey_A5_b76* track;
    UnkSmMachine_A5_b73 machine;
    u8 pad_0x230[0x540 - 0x230];
    base::UnkDeferredSet<void>* watch;
    u8 pad_0x548[0x5a8 - 0x548];
    bool on;
};
static_assert(__builtin_offsetof(UnkPlatformSelf_A5_b76, machine) == 0x180, "layout");
static_assert(__builtin_offsetof(UnkPlatformSelf_A5_b76, on) == 0x5a8, "layout");

static inline UnkSmartObjectActor* LinkedObject_A5_b76(UnkPlatformSmartLink_A5_b76* link) {
    return link ? link->object : nullptr;
}

void fn_7100a09948(UnkPlatformSelf_A5_b76* self) {
    self->vf_0x140(reinterpret_cast<char*>(g_unk_7101d47310) + 0x968);
    if (!self->on) {
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->entity), &g_unk_710164b4c0, false);
        self->on = true;
    }
    UnkSmState_A5_b73* current = self->machine.current;
    if (!current || current->id != 7) {
        if (UnkSmartObjectActor* object = LinkedObject_A5_b76(self->link)) {
            auto* user = reinterpret_cast<UnkPlatformSmartUser_A5_b76*>(fn_71007cfd8c(object));
            if (!user->vf_0x180())
                RequestState_A5_b73<7, 6>(&self->machine, 3);
            if (!user->busy) {
                if (UnkSmartObjectActor* o = LinkedObject_A5_b76(self->link)) {
                    fn_71007cfd8c(o);
                    UnkPlatformSlotsOwner_A5_b76* owner = self->entity->slots->owner;
                    CVector4D color;
                    color.x = 1.0f;
                    color.y = 1.0f;
                    color.z = 1.0f;
                    color.w = 1.0f;
                    fn_71001c287c(owner->slots, self->slot, 0, &color, 1);
                    color.w = 0.0f;
                    u32 slot = self->slot;
                    fn_71001c2764(owner->slots, slot, 0, &color, 1, true);
                }
                if (UnkSmartObjectActor* o = LinkedObject_A5_b76(self->link)) {
                    fn_71007cfd8c(o);
                    fn_710114ffb8(self->entity->control, self->track->hash, 0, 0);
                }
            }
        }
        current = self->machine.current;
        if (!current || current->id == 6) {
            fn_7100d7c828(reinterpret_cast<UnkActor*>(self->entity), &g_unk_710164b4c8, false);
            self->on = false;
            fn_7100a09cd4(reinterpret_cast<UnkWeightPlatform_b49a4*>(self));
        }
    }
    if (void* part = self->entity->part)
        fn_7100861fc8(part, false);
    base::UnkDeferredSet<void>** slot = &self->watch;
    char* manager = reinterpret_cast<char*>(g_unk_7101d47310);
    if (*slot) {
        fn_71004f3ea4(*slot, slot);
        *slot = nullptr;
    }
    *slot = reinterpret_cast<base::UnkDeferredSet<void>*>(manager + 0x1da8);
    fn_71004f3cf0(*slot, slot);
}
