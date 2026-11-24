// CAbilityOpticCamouflage (unit 432): after the ability base step (fn_71002f9de8), marks +0x390 and, in
// state 6, moves the listener at +0x3a0 from the gun component it watched to the actor's current one
// (+0x2a8), then clears the flag at +0x35c of the actor's +0x228 part. Names and layouts are placeholders.
#include "game/CGunComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkAbility_b60a2;
struct UnkListener_b40;

void fn_71002f9de8(UnkAbility_b60a2* self, void*);
void CAbilityOpticCamouflage_GetWeakHandle(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710083b0b4(void*, void*);

struct UnkOpticState_l3239e0 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkOpticFlags_l3239e0 {
    u8 pad_0x0[0x35c];
    bool field_0x35c;
};

struct UnkOpticPartInner_l3239e0 {
    u8 pad_0x0[0x138];
    UnkOpticFlags_l3239e0* flags;  // 0x138
};

struct UnkOpticPart_l3239e0 {
    u8 pad_0x0[0x50];
    UnkOpticPartInner_l3239e0* inner;  // 0x50
};

struct UnkOpticActor_l3239e0 {
    u8 pad_0x0[0x228];
    UnkOpticPart_l3239e0* part;  // 0x228
    u8 pad_0x230[0x78];
    UnkWeakTarget* gun;  // 0x2a8
};

struct UnkOpticOwner_l3239e0 {
    u8 pad_0x0[0x30];
    UnkOpticActor_l3239e0* actor;  // 0x30
};

// Watches a gun component through the weak handle at +0x18.
struct UnkOpticListener_l3239e0 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakHandle* link;  // 0x18
};

struct UnkOpticAbility_l3239e0 {
    u8 pad_0x0[0x20];
    UnkOpticState_l3239e0* sub_state;  // 0x20
    u8 pad_0x28[0x8c];
    s32 state;  // 0xb4
    u8 pad_0xb8[0x8];
    UnkOpticOwner_l3239e0* owner;  // 0xc0
    u8 pad_0xc8[0x2c8];
    s32 field_0x390;
    u8 pad_0x394[0xc];
    UnkOpticListener_l3239e0 listener;  // 0x3a0
};
static_assert(__builtin_offsetof(UnkOpticAbility_l3239e0, field_0x390) == 0x390, "layout");
static_assert(__builtin_offsetof(UnkOpticAbility_l3239e0, listener) == 0x3a0, "layout");

void fn_71003239e0(UnkOpticAbility_l3239e0* self, void* other) {
    fn_71002f9de8(reinterpret_cast<UnkAbility_b60a2*>(self), other);
    self->field_0x390 = 1;
    s32 state = self->state;
    if (state == 7) {
        if (!self->sub_state)
            return;
        state = self->sub_state->kind;
    }
    if (state != 6)
        return;
    if (UnkWeakHandle* handle = self->listener.link) {
        if (handle->target)
            fn_710083b0bc(handle->target, reinterpret_cast<UnkListener_b40*>(&self->listener));
        ResetRef(&self->listener.link);
    }
    if (UnkWeakTarget* gun = self->owner->actor->gun) {
        CAbilityOpticCamouflage_GetWeakHandle(gun, &self->listener.link);
        fn_710083b0b4(self->listener.link ? self->listener.link->target : nullptr, &self->listener);
    }
    if (UnkOpticPart_l3239e0* part = self->owner->actor->part) {
        if (UnkOpticPartInner_l3239e0* inner = part->inner)
            inner->flags->field_0x35c = false;
    }
}
