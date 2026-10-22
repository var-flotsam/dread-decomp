// A GUI handler: after the base handling (fn_71011cf75c), an event of kind 2 runs
// fn_7100b73624 and then fn_7100b72e5c, with the flag at 0x468 when the composition's flag at
// 0x3c and the object at 0x330 are both set (else false).
#include "types.h"

struct UnkGuiEvent {
    u8 unk_0x0[0x20];
    u64 kind;
};

struct UnkComposition {
    u8 unk_0x0[0x3c];
    bool field_0x3c;
};

struct UnkGuiScreen {
    u8 unk_0x0[0x330];
    void* field_0x330;
    u8 unk_0x338[0x130];
    u8 field_0x468;
};

void fn_71011cf75c(UnkGuiScreen* self, UnkGuiEvent* event);
void fn_7100b73624(UnkGuiScreen* self);
UnkComposition* fn_71011cde14(UnkGuiScreen* self);
void fn_7100b72e5c(UnkGuiScreen* self, bool flag);

void fn_7100b73ff8(UnkGuiScreen* self, UnkGuiEvent* event) {
    fn_71011cf75c(self, event);
    if (event->kind != 2)
        return;
    fn_7100b73624(self);
    fn_7100b72e5c(self, fn_71011cde14(self)->field_0x3c && self->field_0x330 && self->field_0x468 != 0);
}
