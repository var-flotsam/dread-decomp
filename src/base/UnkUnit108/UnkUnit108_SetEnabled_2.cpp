// Unit 108 of tools/tu_map.py: the direct form of the reflected setter in UnkUnit108_SetEnabled.cpp:
// stores the flag at +0x2c and, when set, moves state 1 to 3 and 2 to 4, then runs fn_7100119c1c.
// Names and layouts are placeholders.
#include "game/UnkTU108.h"
#include "types.h"

struct UnkTargetState108 {
    u8 pad_0x0[0x2c];
    bool enabled;  // 0x2c
    u8 pad_0x2d[0x13];
    u32 state;  // 0x40
};

struct UnkTarget108;  // src/base/UnkUnit108/UnkUnit108_SetEnabled.cpp

void fn_7100119f98(UnkTargetState108* self, bool enabled) {
    self->enabled = enabled;
    if (enabled) {
        if (self->state == 2)
            self->state = 4;
        else if (self->state == 1)
            self->state = 3;
    }
    fn_7100119c1c(reinterpret_cast<UnkTarget108*>(self));
}
