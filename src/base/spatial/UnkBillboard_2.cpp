// From unit 243 of tools/tu_map.py (see UnkBillboard.cpp): a setter for the billboard's visible flag
// at +0x371, skipped while the lock byte at +0x375 is set and mirrored to the drawable at +0x1a8.
// Names and layouts are placeholders.
#include "types.h"

struct UnkBillboardFlagTarget {
    u8 pad_0x0[0xa8];
    bool field_0xa8;
};
struct UnkBillboardFlags {
    u8 pad_0x0[0x1a8];
    UnkBillboardFlagTarget* field_0x1a8;
    u8 pad_0x1b0[0x371 - 0x1b0];
    bool field_0x371;
    u8 pad_0x372[3];
    bool field_0x375;
};

UnkBillboardFlags* fn_71001b7100(UnkBillboardFlags* self, bool value) {
    if (!self->field_0x375 && self->field_0x371 != value) {
        self->field_0x371 = value;
        self->field_0x1a8->field_0xa8 = value;
    }
    return self;
}
