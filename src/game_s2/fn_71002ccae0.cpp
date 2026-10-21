// Reset routine: state 1 releases the string id at +0x30 and notifies the target; states 1 and 2 clear the state.
#include "base/global/CStrId.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

struct UnkTarget {
    void** vtable;
};

struct UnkReset {
    u8 pad_0x0[8];
    void* field_0x8;
    u32 field_0x10;
    u32 field_0x14;
    u8 pad_0x18[0x10];
    UnkTarget* field_0x28;
    UnkStrIdTwoStep field_0x30;
    u8 pad_0x38[0x18];
    u32 field_0x50;
};

void fn_71002ccae0(UnkReset* self) {
    u32 state = self->field_0x50;
    if (state == 2) {
        self->field_0x50 = 0;
    } else if (state == 1) {
        self->field_0x30.~UnkStrIdTwoStep();
        if (self->field_0x8) {
            self->field_0x10 = 0;
            reinterpret_cast<void (*)(UnkTarget*, void*)>(self->field_0x28->vtable[9])(self->field_0x28, self->field_0x8);
            self->field_0x14 = 0;
        }
        self->field_0x50 = 0;
    }
}
