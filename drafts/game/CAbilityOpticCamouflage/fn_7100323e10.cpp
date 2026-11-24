// CAbilityOpticCamouflage (unit 432): the cloak's start. Lets the actor's movement at +0x208 know
// (fn_71009eaa88), runs the ability base start (fn_71002f9f18), sets the player movement's flag, restarts
// the colour pulse and moves the listener at +0x3a0 to the actor's current gun component (+0x2a8), then
// clears the flag at +0x35c of the actor's +0x228 part. Names and layouts are placeholders.
#include "game/CAbility.h"
#include "game/CGunComponent.h"
#include "game/UnkTU1501.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkListener_b40;
struct UnkPlayerMovement;

void fn_71010d7c8c(UnkPlayerMovement* self, bool value);
void CAbilityOpticCamouflage_GetWeakHandle(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710083b0b4(void*, void*);

struct UnkOpticFlags_l323e10 {
    u8 pad_0x0[0x35c];
    bool field_0x35c;
};

struct UnkOpticPartInner_l323e10 {
    u8 pad_0x0[0x138];
    UnkOpticFlags_l323e10* flags;  // 0x138
};

struct UnkOpticPart_l323e10 {
    u8 pad_0x0[0x50];
    UnkOpticPartInner_l323e10* inner;  // 0x50
};

struct UnkOpticActor_l323e10 {
    u8 pad_0x0[0x200];
    UnkPlayerMovement* player_movement;  // 0x200
    void* movement;                      // 0x208
    u8 pad_0x210[0x18];
    UnkOpticPart_l323e10* part;  // 0x228
    u8 pad_0x230[0x78];
    UnkWeakTarget* gun;  // 0x2a8
};

struct UnkOpticOwner_l323e10 {
    u8 pad_0x0[0x30];
    UnkOpticActor_l323e10* actor;  // 0x30
};

// Watches a gun component through the weak handle at +0x18.
struct UnkOpticListener_l323e10 {
    void** vtable;
    u8 pad_0x8[0x10];
};

struct UnkOpticAbility_l323e10 {
    u8 pad_0x0[0xc0];
    UnkOpticOwner_l323e10* owner;  // 0xc0
    u8 pad_0xc8[0x2cc];
    f32 phase;  // 0x394
    u8 pad_0x398[0x8];
    UnkOpticListener_l323e10 listener;  // 0x3a0
    UnkWeakHandle* link;                // 0x3b8
};
static_assert(__builtin_offsetof(UnkOpticAbility_l323e10, phase) == 0x394, "layout");
static_assert(__builtin_offsetof(UnkOpticAbility_l323e10, listener) == 0x3a0, "layout");

void fn_7100323e10(UnkOpticAbility_l323e10* self, s32 kind) {
    if (void* movement = self->owner->actor->movement)
        fn_71009eaa88(movement);
    fn_71002f9f18(reinterpret_cast<UnkSmLockon_A1_b64*>(self), kind);
    fn_71010d7c8c(self->owner->actor->player_movement, true);
    self->phase = 0.0f;
    UnkWeakTarget* gun = self->owner->actor->gun;
    UnkOpticListener_l323e10* listener = &self->listener;
    if (UnkWeakHandle* handle = self->link) {
        if (handle->target) {
            fn_710083b0bc(handle->target, reinterpret_cast<UnkListener_b40*>(listener));
            if (!self->link)
                goto attach;
        }
        ResetRef(&self->link);
    }
attach:
    if (gun) {
        CAbilityOpticCamouflage_GetWeakHandle(gun, &self->link);
        fn_710083b0b4(self->link ? self->link->target : nullptr, listener);
    }
    if (UnkOpticPart_l323e10* part = self->owner->actor->part) {
        if (UnkOpticPartInner_l323e10* inner = part->inner)
            inner->flags->field_0x35c = false;
    }
}
