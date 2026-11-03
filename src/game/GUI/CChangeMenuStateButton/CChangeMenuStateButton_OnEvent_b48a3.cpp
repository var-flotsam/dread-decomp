// GUI::CChangeMenuStateButton (tools/tu_map.py; the name from reflection): after the base handler
// (fn_71011cf39c), events of kinds 7, 8, 15 and 16 copy the flag at +0xb5 of the object at +0x260 into
// +0x248. Names and layout are placeholders.
#include "types.h"

struct UnkGuiEvent_b48a3 {
    u8 pad_0x0[0x8];
    u32 kind;
};

struct UnkMenuStateSource_b48a3 {
    u8 pad_0x0[0xb5];
    bool flag;
};

struct UnkChangeMenuStateButton_b48a3 {
    u8 pad_0x0[0x248];
    bool flag;
    u8 pad_0x249[0x260 - 0x249];
    UnkMenuStateSource_b48a3* source;
};

void fn_71011cf39c(UnkChangeMenuStateButton_b48a3* self, UnkGuiEvent_b48a3* event);

void fn_7100af5968(UnkChangeMenuStateButton_b48a3* self, UnkGuiEvent_b48a3* event) {
    fn_71011cf39c(self, event);
    switch (event->kind) {
    case 7:
    case 8:
    case 15:
    case 16:
        self->flag = self->source->flag;
        break;
    default:
        break;
    }
}
