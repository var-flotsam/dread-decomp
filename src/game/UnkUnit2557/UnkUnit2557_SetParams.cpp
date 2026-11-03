// From unit 2557 of tools/tu_map.py (0x7100e0f028-0x7100e140c0, no reflected class): copies a 0x34-byte block
// of parameters into the object at 0x1ec. Names and layouts are placeholders.
#include "types.h"

struct UnkUnit2557Params {
    u32 words[13];
};

struct UnkUnit2557Object {
    u8 pad_0x0[0x1ec];
    UnkUnit2557Params params;
};

void fn_7100e1386c(UnkUnit2557Object* self, const UnkUnit2557Params* params) {
    self->params = *params;
}
