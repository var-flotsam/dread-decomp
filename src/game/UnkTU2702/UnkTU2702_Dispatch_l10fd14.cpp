// Unit 2702: an AI component picks the handler for the state of its owner's track and tail-calls it. Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkDispatchAI_l10fd14;
struct UnkDispatchVtable_l10fd14 {
    void* pad_0x0[0xec];
    void (*vf_0x760)(UnkDispatchAI_l10fd14* self);
    void (*vf_0x768)(UnkDispatchAI_l10fd14* self);
    void (*vf_0x770)(UnkDispatchAI_l10fd14* self);
    void* pad_0x778[0x5];
    void (*vf_0x7a0)(UnkDispatchAI_l10fd14* self);
};

struct UnkDispatchMovement_l10fd14 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkDispatchOwner_l10fd14 {
    u8 pad_0x0[0x1f0];
    UnkDispatchMovement_l10fd14* movement;  // 0x1f0
    u8 pad_0x1f8[0x28];
    UnkAnimationComponent* track_owner;  // 0x220
};

struct UnkDispatchAI_l10fd14 {
    UnkDispatchVtable_l10fd14* vtable;
    u8 pad_0x8[0x28];
    UnkDispatchOwner_l10fd14* owner;  // 0x30
};

struct UnkDispatchState_l10fd14 {
    u32 state;
};

void fn_710110fd14(UnkDispatchAI_l10fd14* self, UnkDispatchState_l10fd14* state, bool arg) {
    UnkAnimationComponent* track = self->owner->track_owner;
    bool turn = false;
    bool push_or_crazy = false;
    bool fall = false;
    if (track) {
        push_or_crazy = fn_710117b348(track, fn_7100d253f8()) || fn_710117b348(track, fn_7100cf3508());
        turn = fn_710117b348(track, fn_7100d2b3a8());
        fall = fn_710117b348(track, fn_7100cf5d98()) || fn_710117b348(track, fn_7100cf5888());
    }
    if (!(turn || push_or_crazy || fall) && arg)
        self->vtable->vf_0x7a0(self);
    if (state->state != 2)
        self->vtable->vf_0x770(self);
    else if (self->owner->movement->field_0xf9)
        self->vtable->vf_0x760(self);
    else
        self->vtable->vf_0x768(self);
}
