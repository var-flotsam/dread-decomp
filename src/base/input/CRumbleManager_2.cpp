// From unit 51 of tools/tu_map.py (base::input::CRumbleManager; base/input/UnkRumbleManager.h): stops
// (fn_7100072328 with 0.2 and 0) every rumble in the array at +0x2158 whose channel bit (1 << the
// word at +0x54) is set in the given mask. Names are placeholders.
#include "base/input/UnkRumbleManager.h"
#include "types.h"

struct UnkRumble {
    u8 pad_0x0[0x54];
    u32 channel;
};

void fn_7100072328(void* rumble, f32 time, f32 value);

void fn_7100074f94(base::input::UnkRumbleManager* self, const u32* mask) {
    u32 count = *reinterpret_cast<u32*>(&self->field_0x2160);
    if (count == 0)
        return;
    UnkRumble** it = reinterpret_cast<UnkRumble**>(self->field_0x2158);
    UnkRumble** end = it + count;
    for (; it != end; ++it) {
        UnkRumble* rumble = *it;
        u32 bit = 1u << rumble->channel;
        if (*mask & bit)
            fn_7100072328(rumble, 0.2f, 0.0f);
    }
}
