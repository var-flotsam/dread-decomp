// The neighbouring unit's switch (unit 133 of tools/tu_map.py; fn_7100146e60 next door is unit
// 134's, game/UnkSwitchObject.h): unless the object is busy or already in that state, hands the flag to
// the sub-object at +0x28 and, when turning on, resets the one at +0x168; then records the state
// (0 on, 1 off).
#include "base/UnkCloneCalls.h"
#include "game/UnkSwitchObject.h"
#include "types.h"

void fn_7100208004(void* obj, bool flag);

void fn_7100145e94(UnkSwitchObject* self, bool enable) {
    // The state is tested as a byte (any non-zero value is on).
    bool on = *reinterpret_cast<const u8*>(&self->field_0x121) != 0;
    if (!self->field_0x240 && on != enable) {
        fn_7100208004(self->field_0x28, enable);
        if (enable)
            fn_7100025a88(self->field_0x168);
    }
    self->field_0x244 = !enable;
}
