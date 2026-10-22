// CBigkranXSpitAttack: state 1's update. Reports whether the attack should go on to the next
// state. The vector from the owner to the target is tested against the owner's facing (the third
// column of its matrix): when the target is behind it, a timer (+0xe0) runs down and ends the
// state when it is out; otherwise it is reloaded from the attack's data. The squared distance is
// then compared with one of two ranges of the data, and when it is inside, further checks (an
// owner query and a value of the owner's target against one of the data) decide. A true result
// is cleared when the sub-object at +0x220 of the owner isn't a track that can be aborted.
#include "types.h"

struct UnkSpitMatrix {
    u8 pad_0x0[0x8];
    f32 field_0x8;
    u8 pad_0xc[0xc];
    f32 field_0x18;
    u8 pad_0x1c[0xc];
    f32 field_0x28;
};

struct UnkSpitPoint {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
};

struct UnkSpitOwnerTarget {
    u8 pad_0x0[0x14f8];
    f32 field_0x14f8;
};

struct UnkSpitOwner {
    u8 pad_0x0[0x38];
    u8 field_0x38[0xc];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
    u8 pad_0x68[0x10];
    u8 pad_0x78[0x1a8];
    void* field_0x220;
    u8 pad_0x228[0x18];
    UnkSpitOwnerTarget* field_0x240;
};

struct UnkSpitConfig {
    u8 pad_0x0[0xf0];
    f32 field_0xf0;
    f32 field_0xf4;
    f32 field_0xf8;
    f32 field_0xfc;
};

struct UnkSpitState1 {
    u8 pad_0x0[0x8];
    UnkSpitOwner* field_0x8;
    u8 pad_0x10[0x18];
    UnkSpitConfig* field_0x28;
    u8 pad_0x30[0xb0];
    f32 field_0xe0;
};

UnkSpitPoint* fn_7100d7c990(UnkSpitOwner* owner);
const UnkSpitMatrix* fn_710002614c(void* transform);
bool fn_7100d7bea0(UnkSpitOwner* owner, bool flag);
void* fn_7100c0ccc8();  // CCharClassAttackCanBeAbortedTrack::refl_GetClass
bool fn_710117b348(void* object, void* type);

// NON_MATCHING(minor): same behaviour; the original reads the owner->target vector in another
// order, tests the owner before the result at the end and shares that end between the paths.
bool fn_71003f28c8(UnkSpitState1* self, f32 delta) {
    UnkSpitOwner* owner = self->field_0x8;
    if (!owner)
        return false;
    UnkSpitPoint* target = fn_7100d7c990(owner);
    if (!target)
        return false;
    if (!self->field_0x28)
        return false;
    const UnkSpitMatrix* matrix = fn_710002614c(self->field_0x8->field_0x38);
    f32 facing_x = matrix->field_0x8;
    f32 facing_y = matrix->field_0x18;
    f32 facing_z = matrix->field_0x28;
    owner = self->field_0x8;
    f32 tx = target->field_0x44, ty = target->field_0x54, tz = target->field_0x64;
    f32 ox = owner->field_0x44, oy = owner->field_0x54, oz = owner->field_0x64;
    f32 dx = tx - ox;
    f32 dy = ty - oy;
    f32 dz = tz - oz;
    const f32* range;
    u32 result;
    if (facing_x * dx + facing_y * dy + facing_z * dz < -0.001f) {
        f32 timer = self->field_0xe0;
        if (timer >= 0.001f) {
            timer -= delta;
            self->field_0xe0 = timer;
        }
        if (timer < 0.001f) {
            self->field_0xe0 = 0;
            result = true;
            goto finish;
        }
        range = &self->field_0x28->field_0xf8;
    } else {
        self->field_0xe0 = self->field_0x28->field_0xf0;
        range = &self->field_0x28->field_0xf4;
    }
    if (dx * dx + dy * dy + dz * dz > *range * *range) {
        result = true;
    } else {
        bool busy = fn_7100d7bea0(owner, true);
        owner = self->field_0x8;
        result = !busy && owner->field_0x240 && owner->field_0x240->field_0x14f8 > self->field_0x28->field_0xfc;
    }
finish:
    if (owner && result) {
        if (void* track = owner->field_0x220) {
            if (!fn_710117b348(track, fn_7100c0ccc8()))
                result = 0;
        }
    }
    return result != 0;
}
