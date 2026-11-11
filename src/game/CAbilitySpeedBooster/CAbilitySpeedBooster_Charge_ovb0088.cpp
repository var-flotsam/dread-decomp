// CAbilitySpeedBooster's unit (tools/tu_map.py): the ability's charge step and its destructors. The step
// (fn_710033bc80) cancels the boost (fn_710033c350) when fn_710033c174 says it may not run; otherwise it adds
// the frame time to the charge (+0x398) while a CCharClassAllowSpeedBoosterTrack the class plays allows it
// (its record's flag at +0x18; allowed without one), starting the boost (+0x394, the timeline key
// g_unk_7101649008) when the charge is still zero and a start is pending (+0x3a1, cleared every step).
// The destructors restore the vtable, destroy the slot at +0x5a8 (fn_7100030a50), three value holders
// (+0x458, 0x70 bytes each, as src/game/CAbilityOpticCamouflage/UnkReflValueHolderDtors_b42.cpp) and the
// member at +0x3a8, then run CAbility's destructor (fn_71002fa2bc), inlined in the deleting one as in
// src/game/CAbilityOpticCamouflage/CAbilityOpticCamouflage_Cloak_ovb0059.cpp. Names and layouts are
// placeholders. The end of a boost (fn_710033b7c4, in state 6) resets the model colour of the player and
// of the camera's actor (CSamusModelUpdaterComponent, fn_710093adc0), plays the stop key on the owner, clears
// two tracks of the other actor's class and, while the boost state (+0x3b8) is 2, one of the owner's; then
// the base end fn_71002f9de8 runs. The glow (fn_710033bf64), while boosting or charged past the tunable's
// +0x24, advances a phase (+0x390) at 10 or 20 per second by the movement's speed and blends the model
// colour between two pairs of colours.
#include <math.h>

#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/CGameObject.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU39.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

extern char g_unk_7101a23798[];  // the ability's vtable
extern char g_unk_7101a23a20[];  // the vtable of the member at +0x3a8
extern void* g_unk_7101a225f8[];  // CAbility's value holders' vtable
extern void* g_unk_7101a22650[];  // the vtable of CAbility's member at +0x10
struct UnkCameraCtrl_ovb0049;

struct UnkMoveSlot_0xb0;
struct UnkOpticAbility_ovb0059;

struct UnkClearedPair_ovb0088 {
    u64 a, b;
    ~UnkClearedPair_ovb0088() {
        a = 0;
        b = 0;
    }
};

struct UnkSpeedBoosterHolder_ovb0088 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    UnkClearedPair_ovb0088 pairs[3];
    u8 pad_0x60[0x10];

    __attribute__((always_inline)) void Destroy() {
        for (int i = 2; i >= 0; --i)
            pairs[i].~UnkClearedPair_ovb0088();
        vtable = g_unk_7101a239c8 + 0x10;
        fn_71000a1c60(&value);
    }
};
static_assert(sizeof(UnkSpeedBoosterHolder_ovb0088) == 0x70, "holder size");

// CAbility's value holders (as src/game/CAbilityOpticCamouflage/CAbilityOpticCamouflage_Cloak_ovb0059.cpp).
struct UnkAbilityHolder_ovb0088 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    UnkClearedPair_ovb0088 pairs[4];

    __attribute__((always_inline)) void Destroy() {
        for (int i = 3; i >= 0; --i)
            pairs[i].~UnkClearedPair_ovb0088();
        vtable = reinterpret_cast<char*>(&g_unk_7101a225f8[2]);
        fn_71000a1c60(&value);
    }
};
static_assert(sizeof(UnkAbilityHolder_ovb0088) == 0x70, "holder size");

struct UnkAbilityMember_ovb0088 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
};

struct UnkSpeedBoosterMember_ovb0088 {
    char* vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    ~UnkSpeedBoosterMember_ovb0088() {
        vtable = g_unk_7101a23a20 + 0x10;
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};

struct UnkSpeedBooster_ovb0088 {
    char* vtable;
    u64 field_0x8;
    UnkAbilityMember_ovb0088 member_0x10;  // 0x10
    u8 pad_0x38[0xb0];
    UnkAbilityHolder_ovb0088 ability_holders[6];  // 0xe8
    u8 pad_0x388[0x20];
    UnkSpeedBoosterMember_ovb0088 member;  // 0x3a8
    u8 pad_0x3d0[0x88];
    UnkSpeedBoosterHolder_ovb0088 holders[3];  // 0x458
    u8 slot[0xb0];  // 0x5a8
};
static_assert(__builtin_offsetof(UnkSpeedBooster_ovb0088, holders) == 0x458, "layout");
static_assert(__builtin_offsetof(UnkSpeedBooster_ovb0088, slot) == 0x5a8, "layout");

static_assert(__builtin_offsetof(UnkSpeedBooster_ovb0088, member) == 0x3a8, "layout");

static inline __attribute__((always_inline)) void DestroySpeedBooster(UnkSpeedBooster_ovb0088* self) {
    self->vtable = g_unk_7101a23798 + 0x10;
    fn_7100030a50(reinterpret_cast<UnkMoveSlot_0xb0*>(self->slot));
    self->holders[2].Destroy();
    self->holders[1].Destroy();
    self->holders[0].Destroy();
    self->member.~UnkSpeedBoosterMember_ovb0088();
}

// The deleting destructor, with CAbility's destructor (fn_71002fa2bc) inlined: its vtable, its six value
// holders (last first) and the member at +0x10, then fn_7101043934.
void fn_710033f194(UnkSpeedBooster_ovb0088* self) {
    DestroySpeedBooster(self);
    self->vtable = reinterpret_cast<char*>(&g_unk_7101a223f8[2]);
    for (int i = 5; i >= 0; --i)
        self->ability_holders[i].Destroy();
    self->member_0x10.vtable = reinterpret_cast<char*>(&g_unk_7101a22650[2]);
    self->member_0x10.field_0x20 = 0;
    self->member_0x10.field_0x10 = 0;
    self->member_0x10.field_0x18 = 0;
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
    fn_7100083c00(self);
}

// The complete destructor: the members, then CAbility's destructor.
void fn_710033f0c8(UnkSpeedBooster_ovb0088* self) {
    DestroySpeedBooster(self);
    fn_71002fa2bc(reinterpret_cast<UnkOpticAbility_ovb0059*>(self));
}

extern const u64 g_unk_7101649008;  // the timeline key of the boost's start

struct UnkSpeedBoosterRecord_ovb0088 {
    u8 pad_0x0[0x18];
    bool allow;  // 0x18
};

struct UnkSpeedBoosterOwner_ovb0088 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
};

struct UnkSpeedBoosterAbility_ovb0088 {
    void** vtable;
    u8 pad_0x8[0xb8];
    UnkSpeedBoosterOwner_ovb0088* owner;  // 0xc0
    u8 pad_0xc8[0x2cc];
    bool active;    // 0x394
    u8 pad_0x395[3];
    f32 charge;     // 0x398
    u8 pad_0x39c[5];
    bool starting;  // 0x3a1
};
static_assert(__builtin_offsetof(UnkSpeedBoosterAbility_ovb0088, charge) == 0x398, "layout");

void fn_710033c350(UnkSpeedBoosterAbility_ovb0088* self);
struct UnkSmAbility2_A2_b63;

void fn_710033bc80(UnkSpeedBoosterAbility_ovb0088* self, f32 dt) {
    if (fn_710033c174(reinterpret_cast<UnkSmAbility2_A2_b63*>(self))) {
        UnkTrackQuery query = {fn_7100cea268(), nullptr, -1.0f};
        bool allow;
        if (fn_710117b08c(static_cast<UnkAnimationComponent*>(self->owner->actor->field_0x220), &query))
            allow = reinterpret_cast<UnkSpeedBoosterRecord_ovb0088*>(query.result)->allow;
        else
            allow = true;
        if (self->charge == 0.0f) {
            if (self->starting) {
                self->active = true;
                fn_7100d7c828(self->owner->actor, &g_unk_7101649008, false);
            }
            if (allow && self->active)
                self->charge += dt;
        } else if (allow) {
            self->charge += dt;
        }
    } else {
        fn_710033c350(self);
    }
    self->starting = false;
}

// ---- The end of a boost ----

using base::reflection::UnkReflObject;

struct UnkOwner;
UnkOwner* fn_7100defcd4(UnkGameManager*);
void fn_710093adc0(base::reflection::UnkReflObject*, const void*, const void*, f32);
struct UnkActorTarget;
void fn_7101178e08(UnkActorTarget*, u32, u32, u32, f32);
void fn_710117662c(UnkActorTarget*, u32, u32, u32, u32, f32);
extern const u64 g_unk_7101648ff8;
extern const u64 g_unk_7101649000;
struct UnkV4_b47;
extern const UnkV4_b47 g_unk_7101649044 __attribute__((visibility("hidden")));  // the model colour reset
extern const u8 g_unk_7101649054[] __attribute__((visibility("hidden")));

struct UnkSpeedBoosterEndActor_ovb0088 {
    u8 pad_0x0[0x228];
    UnkReflObject* model_updater;  // 0x228
};

struct UnkSpeedBoosterEndHolder_ovb0088 {
    u8 pad_0x0[0x10];
    UnkSpeedBoosterEndActor_ovb0088* actor;  // 0x10
};

struct UnkSpeedBoosterEndManager_ovb0088 {
    u8 pad_0x0[0x29a0];
    UnkSpeedBoosterEndHolder_ovb0088* holder;  // 0x29a0
};

struct UnkSpeedBoosterEndMovement_ovb0088 {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
};

struct UnkSpeedBoosterEndOwner_ovb0088 {
    u8 pad_0x0[0x30];
    UnkActor* actor;  // 0x30
};

struct UnkSpeedBoosterEndState_ovb0088 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkSpeedBoosterEnd_ovb0088 {
    u8 pad_0x0[0x20];
    UnkSpeedBoosterEndState_ovb0088* sub_state;  // 0x20
    u8 pad_0x28[0x8c];
    s32 state;  // 0xb4
    u8 pad_0xb8[0x8];
    UnkSpeedBoosterEndOwner_ovb0088* owner;  // 0xc0
    u8 pad_0xc8[0x2da];
    bool field_0x3a2;
    u8 pad_0x3a3[0x15];
    UnkSpeedBoosterEndState_ovb0088* boost;  // 0x3b8
};
static_assert(__builtin_offsetof(UnkSpeedBoosterEnd_ovb0088, boost) == 0x3b8, "layout");

void fn_71002f9de8(UnkSpeedBoosterEnd_ovb0088* self, UnkActor* other);

static inline void ResetModel(UnkReflObject* comp) {
    if (comp && fn_710009d50c(comp->vf_0x0(), fn_7100939af4()))
        fn_710093adc0(comp, &g_unk_7101649044, g_unk_7101649054, 0.0f);
}

void fn_710033b7c4(UnkSpeedBoosterEnd_ovb0088* self, UnkActor* other) {
    s32 state = self->state;
    if (state == 7) {
        if (!self->sub_state)
            goto done;
        state = self->sub_state->kind;
    }
    if (state == 6) {
        ResetModel(reinterpret_cast<UnkSpeedBoosterEndActor_ovb0088*>(fn_7100defcd4(g_unk_7101d47310))->model_updater);
        ResetModel(reinterpret_cast<UnkSpeedBoosterEndManager_ovb0088*>(g_unk_7101d47310)->holder->actor->model_updater);
        UnkActor* actor = self->owner->actor;
        if (auto* movement = static_cast<UnkSpeedBoosterEndMovement_ovb0088*>(actor->field_0x1f8))
            fn_7100d7c828(actor, movement->field_0x3a8 ? &g_unk_7101648ff8 : &g_unk_7101649000, false);
        if (other && other->field_0x208 && other->field_0x220) {
            __builtin_memset(&self->field_0x3a2, 0, sizeof(bool));  // untyped: the class pointer is reloaded
            fn_7101178e08(static_cast<UnkActorTarget*>(other->field_0x220), 3, 0, 0, -1.0f);
            fn_7101178e08(static_cast<UnkActorTarget*>(other->field_0x220), 2, 0, 0, -1.0f);
        }
        if (self->boost && self->boost->kind == 2 && actor && actor->field_0x208 && actor->field_0x220)
            fn_710117662c(static_cast<UnkActorTarget*>(actor->field_0x220), 2, 0, 1, 1, 0.0f);
    }
done:
    fn_71002f9de8(self, other);
}

// ---- The glow ----

struct UnkV4_b47 {
    f32 x, y, z, w;
    UnkV4_b47() {}
    UnkV4_b47(f32 x_, f32 y_, f32 z_, f32 w_) : x(x_), y(y_), z(z_), w(w_) {}
    UnkV4_b47(const UnkV4_b47& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

static inline UnkV4_b47 operator*(const UnkV4_b47& a, f32 s) { return fn_7100030830(&a, s); }
static inline UnkV4_b47 operator+(const UnkV4_b47& a, const UnkV4_b47& b) { return fn_7100030770(&a, &b); }

extern base::reflection::UnkTunable g_unk_71726b0fc0;
extern const UnkV4_b47 g_unk_7101647c3c __attribute__((visibility("hidden")));  // the glow colours
extern const UnkV4_b47 g_unk_7101647b4c __attribute__((visibility("hidden")));
extern const UnkV4_b47 g_unk_7101647b7c __attribute__((visibility("hidden")));

struct UnkSpeedBoosterGlowTunable_ovb0088 {
    u8 pad_0x0[0x24];
    f32 glow_charge;  // 0x24
};

struct UnkSpeedBoosterGlowMovement_ovb0088 {
    u8 pad_0x0[0x118];
    f32 speed;  // 0x118
};

struct UnkSpeedBoosterGlowActor_ovb0088 {
    u8 pad_0x0[0x200];
    UnkSpeedBoosterGlowMovement_ovb0088* movement;  // 0x200
    u8 pad_0x208[0x20];
    UnkReflObject* model_updater;  // 0x228
};

struct UnkSpeedBoosterGlowOwner_ovb0088 {
    u8 pad_0x0[0x30];
    UnkSpeedBoosterGlowActor_ovb0088* actor;  // 0x30
};

struct UnkSpeedBoosterGlowState_ovb0088 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkSpeedBoosterGlow_ovb0088 {
    u8 pad_0x0[0x20];
    UnkSpeedBoosterGlowState_ovb0088* sub_state;  // 0x20
    u8 pad_0x28[0x8c];
    s32 state;  // 0xb4
    u8 pad_0xb8[0x8];
    UnkSpeedBoosterGlowOwner_ovb0088* owner;  // 0xc0
    u8 pad_0xc8[0x2c8];
    f32 phase;   // 0x390
    bool active;  // 0x394
    u8 pad_0x395[3];
    f32 charge;  // 0x398

    // CAbility's state: 7 defers to the ability at +0x20.
    s32 State() const {
        if (state == 7)
            return sub_state ? sub_state->kind : 0;
        return state;
    }
    bool IsBoosting() const { return State() == 6; }
};
static_assert(__builtin_offsetof(UnkSpeedBoosterGlow_ovb0088, charge) == 0x398, "layout");

void fn_710033bf64(UnkSpeedBoosterGlow_ovb0088* self, f32 dt) {
    const auto* tunable = reinterpret_cast<const UnkSpeedBoosterGlowTunable_ovb0088*>(&g_unk_71726b0fc0);
    if (!self->IsBoosting() && (!self->active || !(self->charge >= tunable->glow_charge)))
        return;
    UnkSpeedBoosterGlowActor_ovb0088* actor = self->owner->actor;
    if (!actor || !actor->movement)
        return;
    f32 speed = actor->movement->speed > 1.1920929e-07f ? 20.0f : 10.0f;
    self->phase += speed * dt;
    UnkV4_b47 from = self->IsBoosting() ? g_unk_7101649044 : g_unk_7101647c3c;
    UnkV4_b47 to = self->IsBoosting() ? g_unk_7101647b4c : g_unk_7101647b7c;
    UnkV4_b47 zero_b(0.0f, 0.0f, 0.0f, 0.0f);
    UnkV4_b47 zero_a(0.0f, 0.0f, 0.0f, 0.0f);
    f32 t = sinf(self->phase) * 0.5f + 0.5f;
    UnkV4_b47 colour = to * t + from * (1.0f - t);
    UnkV4_b47 extra = zero_a * t + zero_b * (1.0f - t);
    UnkReflObject* comp = actor->model_updater;
    if (comp && !fn_710009d50c(comp->vf_0x0(), fn_7100939af4()))
        comp = nullptr;
    fn_710093adc0(comp, &colour, &extra, -1.0f);
}
