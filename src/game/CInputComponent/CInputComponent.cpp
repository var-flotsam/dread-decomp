// CInputComponent: forwards an input request (fn_7100872460). Optionally resets first
// (fn_7100872268), then hands the argument to the block at +0xad0: fn_710086f870 when `enable`,
// fn_7100870150 otherwise. Names and layouts are placeholders.
#include "game/CInputComponent.h"
#include "game/UnkInputComponent.h"
#include "game/UnkTU1325.h"
#include "types.h"

void fn_7100872460(UnkInputComponent* self, bool enable, bool reset, void* arg) {
    if (reset)
        fn_7100872268(self);
    if (enable)
        fn_710086f870(self->field_0xad0, arg);
    else
        fn_7100870150(self->field_0xad0, arg);
}
