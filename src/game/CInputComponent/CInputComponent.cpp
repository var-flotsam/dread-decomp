// CInputComponent: forwards an input request (fn_7100872460). Optionally resets first
// (fn_7100872268), then hands the argument to the block at +0xad0: fn_710086f870 when `enable`,
// fn_7100870150 otherwise. Names and layouts are placeholders.
#include "types.h"

struct UnkInputComponent {
    u8 pad_0x0[0xad0];
    u8 field_0xad0[1];
};

void fn_7100872268(UnkInputComponent* self);
void fn_710086f870(void* block, void* arg);
void fn_7100870150(void* block, void* arg);

void fn_7100872460(UnkInputComponent* self, bool enable, bool reset, void* arg) {
    if (reset)
        fn_7100872268(self);
    if (enable)
        fn_710086f870(self->field_0xad0, arg);
    else
        fn_7100870150(self->field_0xad0, arg);
}
