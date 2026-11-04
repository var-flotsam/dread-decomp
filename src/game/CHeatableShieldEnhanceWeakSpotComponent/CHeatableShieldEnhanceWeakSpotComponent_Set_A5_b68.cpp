// CHeatableShieldEnhanceWeakSpotComponent's unit: sets the flag at +0x48 (turning it on only when the game manager's
// fn_7100df5b00 allows), then gives the owner's timeline (fn_7100d7c828) the on or off key of the description at
// +0x50: +0x70/+0x80 normally, +0x68/+0x78 when the owner's part (fn_71005012bc) is active (+0x20) and
// fn_710086c89c says no. Names are placeholders.
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkDoorLifeOwner_b56;
struct UnkDoorLifePart_b56;
UnkDoorLifePart_b56* fn_71005012bc(UnkDoorLifeOwner_b56* owner);
bool fn_7100df5b00(UnkGameManager* gm);
bool fn_710086c89c(UnkDoorLifePart_b56* part);

struct UnkWeakSpotDesc_A5_b68 {
    u8 pad_0x0[0x68];
    u64 field_0x68;
    u64 field_0x70;
    u64 field_0x78;
    u64 field_0x80;
};

struct UnkWeakSpot_A5_b68 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x10];
    bool field_0x48;
    u8 pad_0x49[7];
    UnkWeakSpotDesc_A5_b68* field_0x50;
};

void fn_71007d4d10(UnkWeakSpot_A5_b68* self, bool on) {
    if (self->field_0x48 == on)
        return;
    if (on && !fn_7100df5b00(g_unk_7101d47310))
        return;
    self->field_0x48 = on;
    UnkDoorLifePart_b56* part = fn_71005012bc(reinterpret_cast<UnkDoorLifeOwner_b56*>(self->owner));
    if (part && reinterpret_cast<u8*>(part)[0x20] && !fn_710086c89c(part)) {
        UnkWeakSpotDesc_A5_b68* desc = self->field_0x50;
        fn_7100d7c828(self->owner, on ? &desc->field_0x68 : &desc->field_0x78, false);
    } else {
        UnkWeakSpotDesc_A5_b68* desc = self->field_0x50;
        fn_7100d7c828(self->owner, on ? &desc->field_0x70 : &desc->field_0x80, false);
    }
}
