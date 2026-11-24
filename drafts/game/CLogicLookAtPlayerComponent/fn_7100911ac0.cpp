// CLogicLookAtPlayerComponent unit (tools/tu_map.py, unit 1400): the update. Turns the owner's look-at control
// toward the player, or back toward the owner after a delay while the player is gone or cloaked, and aims it at
// the bone the component names (a default one when it names none). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU399.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::CClass;

struct UnkLookAtPosControl_b69;
void CLookAtPosControl_SetTargetPos(UnkLookAtPosControl_b69* self, const CVector3D& pos, f32 time);
void fn_71002e288c(UnkLookAtPosControl_b69* self, u64 bone);
const CStrId& fn_7100323590();

// The key the look-at control is registered under, and the bone it aims at by default.
extern u64 g_unk_7172735a20 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172735a28 __attribute__((visibility("hidden")));

// The owner's look-at control.
struct UnkLookAtControl_l911ac0 {
    virtual const CClass* GetClass();
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
};

// An ability with a state; while the state is 7 its sub-object's state counts.
struct UnkAbilitySubState_l911ac0 {
    u8 pad_0x0[0x10];
    s32 state;
};
struct UnkCloakAbility_l911ac0 {
    virtual const CClass* GetClass();
    u8 pad_0x8[0x18];
    UnkAbilitySubState_l911ac0* sub;
    u8 pad_0x28[0x8c];
    s32 state;

    s32 GetState() const {
        if (state == 7)
            return sub ? sub->state : 7;
        return state;
    }
};
static_assert(__builtin_offsetof(UnkCloakAbility_l911ac0, state) == 0xb4, "layout");

// An actor as this function reads it: two coordinates and the ability set.
struct UnkLookAtActor_l911ac0 {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0x1c8];
    void* field_0x220;
    u8 pad_0x228[0xb8];
    UnkAbilitySet* abilities;  // 0x2e0
};
static_assert(__builtin_offsetof(UnkLookAtActor_l911ac0, abilities) == 0x2e0, "layout");

struct UnkLookAtRegistryOwner_l911ac0 {
    u8 pad_0x0[0x40];
    void* registry;
};

struct UnkLookAtDef_l911ac0 {
    u8 pad_0x0[0x58];
    CStrId bone;
};

struct UnkLogicLookAtPlayer_l911ac0 {
    void* vtable;
    u8 pad_0x8[0x28];
    UnkLookAtActor_l911ac0* owner;  // 0x30
    u8 pad_0x38[0x8];
    UnkLookAtDef_l911ac0* def;  // 0x40
    f32 timer;  // 0x48
    f32 delay;  // 0x4c
    CVector3D target;  // 0x50
};
static_assert(__builtin_offsetof(UnkLogicLookAtPlayer_l911ac0, target) == 0x50, "layout");

static inline UnkLookAtActor_l911ac0* Player() {
    return reinterpret_cast<UnkLookAtActor_l911ac0*>(fn_7100dec788(g_unk_7101d47310));
}

// Whether the player's optic camouflage is on (its state is 6).
static inline bool IsPlayerCloaked() {
    UnkLookAtActor_l911ac0* player = Player();
    if (!player || !player->abilities)
        return false;
    UnkCloakAbility_l911ac0* ability = reinterpret_cast<UnkCloakAbility_l911ac0*>(
        fn_71003012f8(player->abilities, reinterpret_cast<u64>(&fn_7100323590())));
    if (!ability || !fn_710009d50c(ability->GetClass(), fn_7100320270()))
        return false;
    return ability->GetState() == 6;
}

// Moves `value` toward `target` by at most `step`.
static inline void Approach(f32& value, const f32& target, f32 step) {
    if (target > value) {
        f32 limit = step + value;
        value = target < limit ? target : limit;
    } else {
        f32 limit = value - step;
        value = target > limit ? target : limit;
    }
}

void fn_7100911ac0(UnkLogicLookAtPlayer_l911ac0* self, f32 dt) {
    fn_71010c424c(reinterpret_cast<UnkParent*>(self));
    UnkLookAtRegistryOwner_l911ac0* registry_owner =
        static_cast<UnkLookAtRegistryOwner_l911ac0*>(self->owner->field_0x220);
    UnkLookAtControl_l911ac0* control =
        reinterpret_cast<UnkLookAtControl_l911ac0*>(fn_71002d57ac(registry_owner->registry, &g_unk_7172735a20));
    if (control && !fn_710009d50c(control->GetClass(), fn_71002e6474()))
        control = nullptr;

    CStrId bone(base::global::UnkStrIdInline(), nullptr, false);
    bool cloaked = IsPlayerCloaked();
    const CStrId& named = self->def->bone;
    if (named.data != &g_unk_7101d47108->empty)
        bone = named;
    else
        bone = g_unk_7172735a28;

    if (control) {
        UnkLookAtActor_l911ac0* player = Player();
        if (cloaked || !player) {
            if (self->timer >= self->delay) {
                f32 step = dt * 600.0f;
                Approach(self->target.x, self->owner->field_0x44, step);
                Approach(self->target.y, self->owner->field_0x54, step);
                self->target.z = 0.0f;
            } else {
                self->timer += dt;
            }
        } else {
            Approach(self->target.x, Player()->field_0x44, dt * 1000.0f);
            Approach(self->target.y, Player()->field_0x54, dt * 1000.0f);
            self->timer = 0.0f;
        }
        UnkLookAtPosControl_b69* pos_control = reinterpret_cast<UnkLookAtPosControl_b69*>(control);
        CLookAtPosControl_SetTargetPos(pos_control, self->target, -1.0f);
        fn_71002e288c(pos_control, bone.data->Key());
        control->vf_0x78();
    }
}
