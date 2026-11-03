// tools/tu_map.py unit 284 (no class hint): sets the flag +0x70 of the part held at +0x108: on when
// `force` is set, else the inverse of the flag +0xf7. The same code is in unit 275.
// Names are placeholders.
#include "types.h"

struct UnkPart284 {
    u8 pad_0x0[0x70];
    bool field_0x70;
};

struct UnkOwner284 {
    u8 pad_0x0[0xf7];
    bool field_0xf7;
    u8 pad_0xf8[0x10];
    UnkPart284* part;  // 0x108
};

void fn_71002080c8(UnkOwner284* self, bool force) {
    bool value = force || !self->field_0xf7;
    self->part->field_0x70 = value;
}
