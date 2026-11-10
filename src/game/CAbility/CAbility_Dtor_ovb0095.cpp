// CAbility's unit (tools/tu_map.py unit 427): whether the owner's life energy covers the ability's amount
// (as fn_71002f93a8 with the energy from fn_7100a1ff18 of the owner's +0xe8 part, its +0x10) and CAbility's
// destructors (its six value holders at +0xe8, the member at +0x10, then fn_7101043934; inlined in
// CAbilityOpticCamouflage's deleting destructor). Names and layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkLifeComponent;
f32 fn_7100a1ff18(UnkLifeComponent* life);

struct UnkAbilityLifeHolder_ovb0095 {
    u8 pad_0x0[0x10];
    UnkLifeComponent* life;  // 0x10
};

struct UnkAbilityAmountOwner_ovb0095 {
    u8 pad_0x0[0xe8];
    UnkAbilityLifeHolder_ovb0095* holder;  // 0xe8
};

struct UnkAbilityAmount_ovb0095 {
    void** vtable;
    u8 pad_0x8[0xb8];
    UnkAbilityAmountOwner_ovb0095* owner;  // 0xc0
    f32 vf_0xc0() { return reinterpret_cast<f32 (*)(UnkAbilityAmount_ovb0095*)>(vtable[0xc0 / 8])(this); }
};

// Whether the owner's life energy (fn_7100a1ff18) covers the ability's amount (virtual 0xc0): true when
// the amount is zero within 0.001, else when the energy is above 1e-5 and at least the amount.
bool fn_71002f92d4(UnkAbilityAmount_ovb0095* self) {
    UnkAbilityLifeHolder_ovb0095* holder = self->owner->holder;
    if (!holder)
        return false;
    UnkLifeComponent* life = holder->life;
    if (!life)
        return false;
    f32 energy = fn_7100a1ff18(life);
    f32 amount = self->vf_0xc0();
    if (amount + 0.001f >= 0.0f && amount + -0.001f <= 0.0f)
        return true;
    if (energy > 1e-5f)
        return self->vf_0xc0() <= energy;
    return false;
}

extern void* g_unk_7101a223f8[];  // CAbility's vtable
extern void* g_unk_7101a225f8[];  // the value holders' vtable
extern void* g_unk_7101a22650[];  // the vtable of the member at +0x10

struct UnkCameraCtrl_ovb0049;
void fn_7101043934(UnkCameraCtrl_ovb0049*);
struct UnkOpticAbility_ovb0059;

struct UnkClearedPair_ovb0095 {
    u64 a, b;

    ~UnkClearedPair_ovb0095() {
        a = 0;
        b = 0;
    }
};

// A value holder: a reflected value and four pairs cleared on destruction.
struct UnkAbilityValueHolder_ovb0095 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    UnkClearedPair_ovb0095 pairs[4];

    __attribute__((always_inline)) void Destroy() {
        for (int i = 3; i >= 0; --i)
            pairs[i].~UnkClearedPair_ovb0095();
        vtable = &g_unk_7101a225f8[2];
        fn_71000a1c60(&value);
    }
};
static_assert(sizeof(UnkAbilityValueHolder_ovb0095) == 0x70, "holder size");

struct UnkAbilityMember_ovb0095 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
};

struct UnkAbility_ovb0095 {
    void** vtable;
    u64 field_0x8;
    UnkAbilityMember_ovb0095 member;  // 0x10
    u8 pad_0x38[0xb0];
    UnkAbilityValueHolder_ovb0095 holders[6];  // 0xe8

    __attribute__((always_inline)) void DestroyMembers() {
        vtable = &g_unk_7101a223f8[2];
        for (int i = 5; i >= 0; --i)
            holders[i].Destroy();
        member.vtable = &g_unk_7101a22650[2];
        member.field_0x20 = 0;
        member.field_0x10 = 0;
        member.field_0x18 = 0;
        fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(this));
    }
};
static_assert(__builtin_offsetof(UnkAbility_ovb0095, holders) == 0xe8, "layout");

// CAbility's destructor: its six value holders (last first), the member at +0x10, then the base
// destructor fn_7101043934.
void fn_71002fa2bc(UnkOpticAbility_ovb0059* self) {
    reinterpret_cast<UnkAbility_ovb0095*>(self)->DestroyMembers();
}

// The deleting destructor.
void fn_71002fa3f4(UnkAbility_ovb0095* self) {
    self->DestroyMembers();
    operator delete(self);
}
