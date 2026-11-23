// Whether the object may go on (1 or 0): the target at +0x149b8 is still above -0.001 and the
// value at +0x14ba4 is below 0.001, or the mode at +0x14518 is 2; and then the owner (+0x30) has
// nothing at +0x258, or fn_7101136308 refuses it.
#include "game/CAttackComponent.h"
#include "game/UnkSelf.h"
#include "types.h"

struct UnkTarget {
    u8 pad_0x0[0x624];
    f32 field_0x624;
};

struct UnkOwner {
    u8 pad_0x0[0x258];
    void* field_0x258;
};

u32 fn_71003f4bc0(UnkSelf* self) {
    UnkTarget* target = self->field_0x149b8;
    if (!((target && target->field_0x624 > -0.001f && self->field_0x14ba4 < 0.001f) ||
          self->field_0x14518 == 2))
        return false;
    void* object = self->field_0x30->field_0x258;
    if (!object)
        return true;
    return !fn_7101136308(object);
}
