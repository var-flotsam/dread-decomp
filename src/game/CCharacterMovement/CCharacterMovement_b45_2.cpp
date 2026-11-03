// CCharacterMovement's unit (tools/tu_map.py): whether the object the link names (its object at
// +0x10) is the object (+0x10) of one of the entries of the 16-byte array at +0x3f8 (count at
// +0x400). Names are placeholders.
#include "types.h"

struct UnkMovementLinkTarget_b45a2 {
    u8 pad_0x0[0x10];
    void* object;
};

struct UnkMovementSlot_b45a2 {
    UnkMovementLinkTarget_b45a2* target;
    u64 field_0x8;
};

struct UnkCharacterMovementSlots_b45a2 {
    u8 pad_0x0[0x3f8];
    UnkMovementSlot_b45a2* slots;
    u32 count;
};

bool fn_71009790e8(UnkCharacterMovementSlots_b45a2* self, UnkMovementLinkTarget_b45a2** link) {
    if (!*link)
        return false;
    void* object = (*link)->object;
    if (!object)
        return false;
    for (UnkMovementSlot_b45a2 *it = self->slots, *end = it + self->count; it != end; ++it) {
        UnkMovementLinkTarget_b45a2* t = it->target;
        if (t && t->object && object == t->object)
            return true;
    }
    return false;
}
