// Reset routine: state 1 releases the string id at +0x40 and notifies the target; states 1 and 2 clear the state.
#include "base/global/CStrId.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

struct UnkTarget {
    void** vtable;
};

struct UnkReset {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u32 field_0x20;
    u32 field_0x24;
    u8 pad_0x28[0x10];
    UnkTarget* field_0x38;
    UnkStrIdTwoStep field_0x40;
    u8 pad_0x48[0x18];
    u32 field_0x60;
};

void fn_71002cb334(UnkReset* self) {
    u32 state = self->field_0x60;
    if (state == 2) {
        self->field_0x60 = 0;
    } else if (state == 1) {
        self->field_0x40.~UnkStrIdTwoStep();
        if (self->field_0x18) {
            self->field_0x20 = 0;
            reinterpret_cast<void (*)(UnkTarget*, void*)>(self->field_0x38->vtable[9])(self->field_0x38, self->field_0x18);
            self->field_0x24 = 0;
        }
        self->field_0x60 = 0;
    }
}
