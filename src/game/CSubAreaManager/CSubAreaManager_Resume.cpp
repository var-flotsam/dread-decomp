// CSubAreaManager unit: in state 1 (+0x110), when the current sub-area (+0x3f0) asks for it through
// its flag at +0x181 or the mode at +0x108 is already 1, clears +0x10c and runs fn_7100ee62bc on it,
// keeping the mode unchanged. Names and layouts are placeholders (the full ones are in
// src/game/CSubAreaManager.cpp).
#include "types.h"
struct CSubAreaManager;
struct UnkSubArea;
void fn_7100ee62bc(CSubAreaManager* self, UnkSubArea* area);
struct UnkSubAreaView {
    u8 pad_0x0[0x181];
    // A union member (no type-based alias info), as in CSubAreaManager.cpp: reloaded after every store.
    union {
        bool field_0x181;
    };
};
struct UnkSubAreaManagerView {
    u8 pad_0x0[0x108];
    u32 field_0x108;
    u32 field_0x10c;
    u32 field_0x110;
    u8 pad_0x114[0x2dc];
    UnkSubAreaView* field_0x3f0;
};
void fn_7100eeb834(UnkSubAreaManagerView* self) {
    UnkSubAreaView* cur = self->field_0x3f0;
    u32 old = self->field_0x108;
    if (cur->field_0x181)
        self->field_0x108 = 1;
    if (self->field_0x110 == 1 && self->field_0x108 == 1) {
        self->field_0x10c = 0;
        if (cur->field_0x181)
            self->field_0x108 = old;
        fn_7100ee62bc(reinterpret_cast<CSubAreaManager*>(self), reinterpret_cast<UnkSubArea*>(cur));
    }
}
