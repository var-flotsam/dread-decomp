// tools/tu_map.py unit 259 (no class hint): stores a flag of a large owner and, when it changes,
// passes the new mode on to two setters. Names and layouts are placeholders.
#include "game/malloc.h"
#include "types.h"

void fn_7100159420(s32 mode);

struct UnkModeOwner259_b66a1 {
    u8 pad_0x0[0x3183660];
    bool on;  // 0x3183660
};

void fn_71001d73ec(UnkModeOwner259_b66a1* self, bool on) {
    if (self->on == on) {
        return;
    }
    self->on = on;
    fn_7100159420(on ? 2 : 1);
    fn_7100132aa0(on);
}
