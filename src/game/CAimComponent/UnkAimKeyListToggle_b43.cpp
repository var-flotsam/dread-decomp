// Adding a string id to, or removing it from, one of two key lists an object in CAimComponent's
// unit keeps at +0x6e8 and +0x798 (game/UnkGuiRoot.h's UnkGuiKeyList: fn_710002fd10 adds,
// fn_710002ffb4 removes). The owner's name and layout are placeholders.
#include "game/UnkGuiRoot.h"

struct UnkAimKeyListOwner {
    u8 unk_0x0[0x6e8];
    UnkGuiKeyList list_0x6e8;
    u8 unk_0x718[0x80];
    UnkGuiKeyList list_0x798;
};

static inline void SetKey(UnkGuiKeyList* list, bool on, base::global::CStrId* key) {
    if (on)
        fn_710002fd10(list, key);
    else
        fn_710002ffb4(list, key);
}

void fn_71003448d4(UnkAimKeyListOwner* self, bool on, base::global::CStrId* key) { SetKey(&self->list_0x6e8, on, key); }
void fn_71003448ec(UnkAimKeyListOwner* self, bool on, base::global::CStrId* key) { SetKey(&self->list_0x798, on, key); }
