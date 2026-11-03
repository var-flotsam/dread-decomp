// Unit 208: turns particle `index`'s position (or those of a list of indices) by a matrix's rotation (fn_7100077c04), storing
// the change in a second target's array (velocities, by the look of it) and the new position.
// Names and layouts are placeholders inferred from the code.

#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"
#include "types.h"

UnkVec3f fn_7100077c04(const UnkMatrix44& m, const UnkVec3f& v);

struct UnkVec3Target_710016c83c {
    u8 pad_0x0[0x20];
    UnkVec3f* values;
};

struct UnkRotator_710016c83c {
    u64 field_0x0;
    UnkVec3Target_710016c83c* positions;
    UnkMatrix44 rotation;
    UnkVec3Target_710016c83c* deltas;
};

static inline void Rotate(UnkRotator_710016c83c* self, u32 index) {
    const UnkVec3f& old = self->positions->values[index];
    UnkVec3f p = fn_7100077c04(self->rotation, old);
    self->deltas->values[index] = UnkVec3f(p.x - old.x, p.y - old.y, p.z - old.z);
    self->positions->values[index] = p;
}

void fn_710016c83c(UnkRotator_710016c83c* self, u32 index) {
    Rotate(self, index);
}

void fn_710016c8e0(UnkRotator_710016c83c* self, const u32* indices, u32 count) {
    if (!count)
        return;
    do {
        Rotate(self, *indices++);
    } while (--count);
}
