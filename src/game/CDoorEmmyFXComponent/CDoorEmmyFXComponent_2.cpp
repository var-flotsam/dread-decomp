// In the CDoorEmmyFXComponent unit: switches the door's effect (fn_71007b6b38; fn_71007b70d8 is an
// identical copy, declared elsewhere with one parameter). Hands the actor (+0x30) one of two timeline
// ids chosen by the game manager's flag at +0x2441, adds or removes a unit-local id in the key list
// at +0x70 (fn_710002fd10, fn_710002ffb4), passes the state on to fn_71007b5c48 and returns 0. Names and layouts
// are placeholders.
#include "game/UnkGameManager.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

extern base::global::CStrId g_unk_717271a590 __attribute__((visibility("hidden")));

struct UnkDoorEmmyFX {
    u8 pad_0x0[0x30];
    UnkActor* actor;
    u8 pad_0x38[0x38];
    UnkGuiKeyList field_0x70;
};

void fn_71007b5c48(UnkDoorEmmyFX* self, bool on);

u32 fn_71007b6b38(UnkDoorEmmyFX* self, bool on) {
    u64 a = 0x9eae371e1be1ed9dull;
    u64 b = 0x8f9678a4d47dbbbbull;
    bool flag = reinterpret_cast<const u8*>(g_unk_7101d47310)[0x2441];
    fn_7100d7c828(self->actor, flag ? &a : &b, false);
    if (on)
        fn_710002fd10(&self->field_0x70, &g_unk_717271a590);
    else
        fn_710002ffb4(&self->field_0x70, &g_unk_717271a590);
    fn_71007b5c48(self, on);
    return 0;
}
