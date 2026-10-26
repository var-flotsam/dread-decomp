// Switches an object on or off: records a state value, resets two sub-objects when turning
// on, and forwards the flag to an object reached through field_0x160.
#include "base/UnkCloneCalls.h"
#include "game/UnkSwitchObject.h"
#include "types.h"

void fn_7100208bb4(void* obj, bool flag);
void fn_7100192f08(void* obj, bool flag);

static void Reset(UnkSwitchObject* self, bool enable) {
    if (enable) {
        fn_7100208bb4(self->field_0x28, true);
        fn_7100025a88(self->field_0x168);
    }
    self->field_0x244 = 0;
}

void fn_7100146e60(UnkSwitchObject* self, bool enable) {
    if (self->field_0x240) {
        self->field_0x244 = !enable;
        return;
    }
    if (!self->field_0x121 && enable)
        Reset(self, enable);
    fn_7100192f08(self->field_0x160->field_0x78, enable);
    if (self->field_0x244 == 0 && !enable)
        self->field_0x244 = 2;
}
