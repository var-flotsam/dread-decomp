// CGrappleBeamGun's unit (tools/tu_map.py; the name from reflection): with the flag at +0x1890
// clear, true when the link at +0x1888 names no object, else whether the object's part at +0x490
// is in state 1 (false without one). Names are placeholders.
#include "types.h"

struct UnkGrapplePart_b45a2 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkGrappleTarget_b45a2 {
    u8 pad_0x0[0x490];
    UnkGrapplePart_b45a2* part;
};

struct UnkGrappleLink_b45a2 {
    u8 pad_0x0[0x10];
    UnkGrappleTarget_b45a2* target;
};

struct UnkGrappleBeamGun_b45a2 {
    u8 pad_0x0[0x1888];
    UnkGrappleLink_b45a2* link;
    bool field_0x1890;
};

bool fn_710083384c(UnkGrappleBeamGun_b45a2* self) {
    if (self->field_0x1890)
        return false;
    if (!self->link || !self->link->target)
        return true;
    UnkGrapplePart_b45a2* part = self->link->target->part;
    return part ? part->state == 1 : false;
}
