// Five getters of a player ability: through the owner at 0x29b8, its object at 0x10 and that
// object's ability set at 0x2e0, the ability with the given type id is looked up and returned
// if it is of the expected class (CAbilityShinespark, CAbilityGhostAura,
// CAbilityOpticCamouflage, CAbilitySpeedBooster, CAbilityMultiLockon).
#include "types.h"

struct UnkReflType;
struct UnkAbility {
    virtual const UnkReflType* vf_0x0();
};
struct UnkAbilitySet;
struct UnkAbilityOwner {
    u8 unk_0x0[0x2e0];
    UnkAbilitySet* abilities;
};
struct UnkAbilityHolder {
    u8 unk_0x0[0x10];
    UnkAbilityOwner* owner;
};
struct UnkPlayer {
    u8 unk_0x0[0x29b8];
    UnkAbilityHolder* holder;
};

bool fn_710009d50c(const UnkReflType* type, const UnkReflType* base);

template <u64 (*Id)(), UnkAbility* (*Find)(UnkAbilitySet*, u64), const UnkReflType* (*Class)()>
__attribute__((always_inline)) static inline UnkAbility* GetAbility(UnkPlayer* self) {
    UnkAbilityHolder* holder = self->holder;
    if (!holder || !holder->owner)
        return nullptr;
    UnkAbilitySet* abilities = holder->owner->abilities;
    if (!abilities)
        return nullptr;
    UnkAbility* ability = Find(abilities, Id());
    if (!ability)
        return nullptr;
    return fn_710009d50c(ability->vf_0x0(), Class()) ? ability : nullptr;
}

UnkAbility* fn_71003013ec(UnkAbilitySet* set, u64 id);
UnkAbility* fn_71003012f8(UnkAbilitySet* set, u64 id);
u64 fn_7100327d14();
u64 fn_7100308c74();
u64 fn_7100323590();
u64 fn_710033b6a4();
u64 fn_71003182d8();
const UnkReflType* fn_71003255a4();  // CAbilityShinespark::refl_GetClass
const UnkReflType* fn_7100306914();  // CAbilityGhostAura::refl_GetClass
const UnkReflType* fn_7100320270();  // CAbilityOpticCamouflage::refl_GetClass
const UnkReflType* fn_710033a31c();  // CAbilitySpeedBooster::refl_GetClass
const UnkReflType* fn_7100316638();  // CAbilityMultiLockon::refl_GetClass

UnkAbility* fn_7100df5308(UnkPlayer* self) {
    return GetAbility<fn_7100327d14, fn_71003013ec, fn_71003255a4>(self);
}
UnkAbility* fn_7100df56c0(UnkPlayer* self) {
    return GetAbility<fn_7100308c74, fn_71003012f8, fn_7100306914>(self);
}
UnkAbility* fn_7100df57f4(UnkPlayer* self) {
    return GetAbility<fn_7100323590, fn_71003012f8, fn_7100320270>(self);
}
UnkAbility* fn_7100df5928(UnkPlayer* self) {
    return GetAbility<fn_710033b6a4, fn_71003012f8, fn_710033a31c>(self);
}
UnkAbility* fn_7100df59cc(UnkPlayer* self) {
    return GetAbility<fn_71003182d8, fn_71003012f8, fn_7100316638>(self);
}

// Whether the ability is in state 6: its state at 0xb4, or while that is 7 the state of its
// object at 0x20 (when there is one).
struct UnkAbilityState {
    u8 unk_0x0[0x10];
    s32 state;
};
struct UnkStatefulAbility : UnkAbility {
    u8 unk_0x8[0x18];
    UnkAbilityState* sub;
    u8 unk_0x28[0x8c];
    s32 state;

    s32 GetState() const {
        if (state == 7)
            return sub ? sub->state : 7;
        return state;
    }
};

template <u64 (*Id)(), const UnkReflType* (*Class)()>
__attribute__((always_inline)) static inline bool IsInState6(UnkPlayer* self) {
    UnkAbility* ability = GetAbility<Id, fn_71003013ec, Class>(self);
    if (!ability)
        return false;
    return static_cast<UnkStatefulAbility*>(ability)->GetState() == 6;
}

bool fn_7100df5740(UnkPlayer* self) { return IsInState6<fn_7100323590, fn_7100320270>(self); }
bool fn_7100df5874(UnkPlayer* self) { return IsInState6<fn_710033b6a4, fn_710033a31c>(self); }
bool fn_7100df5a4c(UnkPlayer* self) { return IsInState6<fn_7100327d14, fn_71003255a4>(self); }

// Whether the ghost aura's state (fn_7100309568) is 3 to 6.
u32 fn_7100309568(UnkAbility* ability);

bool fn_7100df5630(UnkPlayer* self) {
    UnkAbility* ability = GetAbility<fn_7100308c74, fn_71003013ec, fn_7100306914>(self);
    if (!ability)
        return false;
    return fn_7100309568(ability) - 3 < 4;
}
