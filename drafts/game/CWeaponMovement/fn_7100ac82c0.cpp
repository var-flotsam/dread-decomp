// CWeaponMovement unit: hits `other`. A target boosting (speed booster or shinespark in state 6) is
// asked first through vf_0x2a0; otherwise a copy of the weapon's damage request goes to the owner's
// damage handler (+0x258) or, failing that, to the target's life component. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "game/CAbilityShinespark.h"
#include "game/CAbilitySpeedBooster.h"
#include "game/CWeaponMovement.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// As in src/game/CKraidBouncingCreaturesMovementComponent/CKraidBouncingCreaturesMovementComponent_3.cpp.
struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

static inline UnkWeakTarget* Target(UnkWeakHandle* handle) {
    return handle ? handle->target : nullptr;
}

struct UnkHitParams_lac82c0 {
    s32 values[12];
};

// A counted reference; dropping the last one calls vf_0x10.
struct UnkHitRef_lac82c0 {
    UnkRefObject* object;

    UnkHitRef_lac82c0() : object(nullptr) {}
    ~UnkHitRef_lac82c0() {
        if (UnkRefObject* p = object) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

// What the owner's damage handler takes.
struct UnkHit_lac82c0 {
    UnkHitRef_lac82c0 source;
    UnkHitRef_lac82c0 weapon;
    UnkHitParams_lac82c0 params;
    s32 field_0x40;

    explicit UnkHit_lac82c0(const UnkHitParams_lac82c0& from) : field_0x40(0) {
        for (s32 i = 0; i < 12; ++i)
            params.values[i] = from.values[i];
    }
};
static_assert(sizeof(UnkHit_lac82c0) == 0x48, "layout");

void SetWeakRefAt0ToObjectHandle(UnkHit_lac82c0* hit, void* source);
void fn_710092f0b4(UnkHit_lac82c0* hit, void* weapon);

struct UnkMoverState_lac82c0 {
    u8 pad_0x0[0x10c];
    f32 speed;  // 0x10c
};

struct UnkMover_lac82c0 {
    u8 pad_0x0[0x200];
    UnkMoverState_lac82c0* state;  // 0x200
};

// The weapon's damage request (copied with fn_71008c1090, destroyed with fn_71008c1558).
struct UnkDamageRequest_lac82c0 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    UnkWeakHandle* mover;   // 0x10
    u8 pad_0x18[0x49 - 0x18];
    bool has_direction;  // 0x49
    u8 pad_0x4a[2];
    f32 direction_x;  // 0x4c
    f32 direction_y;  // 0x50
    u8 pad_0x54[0x60 - 0x54];
    void* field_0x60;
    UnkHitParams_lac82c0 params;  // 0x68
    void* field_0x98;
    u8 pad_0xa0[0x100 - 0xa0];

    UnkDamageRequest_lac82c0(const UnkDamageRequest_lac82c0& other) {
        fn_71008c1090(reinterpret_cast<UnkGooplotRequest_b63a3*>(this),
                      reinterpret_cast<const UnkGooplotRequest_b63a3*>(&other));
    }
    ~UnkDamageRequest_lac82c0() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(__builtin_offsetof(UnkDamageRequest_lac82c0, field_0x98) == 0x98, "layout");

struct UnkDamageHandler_lac82c0 {
    void** vtable;

    s32 Take(UnkWeakTarget* mover, UnkHit_lac82c0* hit, void* weapon, bool flag) {
        return reinterpret_cast<s32 (*)(UnkDamageHandler_lac82c0*, UnkWeakTarget*, UnkHit_lac82c0*, void*, bool)>(
            vtable[0x198 / 8])(this, mover, hit, weapon, flag);
    }
};

struct UnkWeaponOwner_lac82c0 {
    u8 pad_0x0[0x258];
    UnkDamageHandler_lac82c0* damage_handler;  // 0x258
};

struct UnkLife_lac82c0 {
    void** vtable;
    u8 pad_0x8[0x68];
    s32 field_0x70;

    s32 Damage(UnkDamageRequest_lac82c0* request) {
        return reinterpret_cast<s32 (*)(UnkLife_lac82c0*, UnkDamageRequest_lac82c0*)>(vtable[0x188 / 8])(this,
                                                                                                    request);
    }
};

struct UnkHitTarget_lac82c0 {
    u8 pad_0x0[0x1b5];
    bool field_0x1b5;
    u8 pad_0x1b6[2];
    UnkLife_lac82c0* life;  // 0x1b8
    u8 pad_0x1c0[0x2e0 - 0x1c0];
    UnkAbilitySet* abilities;  // 0x2e0
};

struct UnkWeaponMovement_lac82c0 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkWeaponOwner_lac82c0* owner;  // 0x30
    u8 pad_0x38[0x230 - 0x38];
    UnkWeakHandle* field_0x230;
    u8 pad_0x238[0x348 - 0x238];
    UnkDamageRequest_lac82c0 request;  // 0x348

    s32 HitBoosting(UnkHitTarget_lac82c0* other, UnkWeakTarget* target) {
        return reinterpret_cast<s32 (*)(UnkWeaponMovement_lac82c0*, UnkHitTarget_lac82c0*, UnkWeakTarget*)>(
            vtable[0x2a0 / 8])(this, other, target);
    }
    void Prepare(UnkDamageRequest_lac82c0* request) {
        reinterpret_cast<void (*)(UnkWeaponMovement_lac82c0*, UnkDamageRequest_lac82c0*)>(vtable[0x280 / 8])(
            this, request);
    }
};
static_assert(__builtin_offsetof(UnkWeaponMovement_lac82c0, request) == 0x348, "layout");

static inline bool IsBoosting(CAbility* ability) {
    return ability && ability->State() == 6;
}

template <const base::reflection::CClass* (*GetClass)()>
static inline CAbility* FindAbility(UnkAbilitySet* abilities, u64 id) {
    CAbility* ability = reinterpret_cast<CAbility*>(fn_71003012f8(abilities, id));
    if (!ability)
        return nullptr;
    return fn_710009d50c(ability->GetClass(), GetClass()) ? ability : nullptr;
}

s32 fn_7100ac82c0(UnkSplash_b59a4* self_, void* other_) {
    auto* self = reinterpret_cast<UnkWeaponMovement_lac82c0*>(self_);
    auto* other = static_cast<UnkHitTarget_lac82c0*>(other_);
    if (!other)
        return 0;

    if (UnkAbilitySet* abilities = other->abilities) {
        CAbility* speed_booster = FindAbility<fn_710033a31c>(abilities, fn_710033b6a4());
        CAbility* shinespark = FindAbility<fn_71003255a4>(abilities, fn_7100327d14());
        if (IsBoosting(speed_booster) || IsBoosting(shinespark)) {
            s32 result = self->HitBoosting(other, Target(self->field_0x230));
            if (result == 1)
                return 2;
            if (result != 3)
                return 0;
        }
    }

    UnkLife_lac82c0* life = other->life;
    if (!life || life->field_0x70 == 2 || other->field_0x1b5)
        return 0;

    UnkDamageRequest_lac82c0 request(self->request);
    s32 result;
    if (self->owner->damage_handler) {
        UnkHit_lac82c0 hit(self->request.params);
        SetWeakRefAt0ToObjectHandle(&hit, self->request.field_0x60);
        fn_710092f0b4(&hit, self->request.field_0x98);
        result = self->owner->damage_handler->Take(Target(self->request.mover), &hit, self->request.field_0x98,
                                                   false);
    } else {
        UnkWeakTarget* mover = Target(self->request.mover);
        UnkMover_lac82c0* state_owner = reinterpret_cast<UnkMover_lac82c0*>(mover);
        UnkWeakTarget* source;
        if (mover && state_owner->state && state_owner->state->speed == 0.0f &&
            (source = Target(self->request.source)) && fn_7100d7be08(source).x != 0.0f) {
            request.has_direction = true;
            f32 x = fn_7100d7be08(Target(self->request.source)).x;
            request.direction_x = (f32)((x > 0.0f) - (x < 0.0f));
            request.direction_y = 1.0f;
        }
        self->Prepare(&request);
        result = life->Damage(&request);
    }
    return result;
}
