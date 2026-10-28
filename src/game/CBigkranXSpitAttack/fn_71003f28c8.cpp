// CBigkranXSpitAttack: state 1's update. Reports whether the attack should go on to the next
// state. The vector from the owner to the target is tested against the owner's facing (the third
// column of its matrix): when the target is behind it, a timer (+0xe0) runs down and ends the
// state when it is out; otherwise it is reloaded from the attack's data. The squared distance is
// then compared with one of two ranges of the data, and when it is inside, further checks (an
// owner query and a value of the owner's target against one of the data) decide. A true result
// is cleared when the sub-object at +0x220 of the owner isn't a track that can be aborted.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

struct UnkSpitOwnerTarget {
    u8 pad_0x0[0x14f8];
    f32 field_0x14f8;
};

bool fn_7100d7bea0(UnkActor* owner, bool flag);

// NON_MATCHING(minor): same behaviour; the original reads the owner->target vector in another
// order, tests the owner before the result at the end and shares that end between the paths.
bool fn_71003f28c8(UnkSpitAttack* self, f32 delta) {
    UnkActor* owner = self->field_0x8;
    if (!owner)
        return false;
    UnkActor* target = fn_7100d7c990(owner);
    if (!target)
        return false;
    if (!self->field_0x28)
        return false;
    const UnkMatrix44& matrix = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->field_0x8->field_0x38));
    f32 facing_x = matrix.m[2];
    f32 facing_y = matrix.m[6];
    f32 facing_z = matrix.m[10];
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
        result = !busy && owner->field_0x240 &&
                 static_cast<UnkSpitOwnerTarget*>(owner->field_0x240)->field_0x14f8 > self->field_0x28->field_0xfc;
    }
finish:
    if (owner && result) {
        if (UnkAnimationComponent* track = static_cast<UnkAnimationComponent*>(owner->field_0x220)) {
            if (!fn_710117b348(track, fn_7100c0ccc8()))
                result = 0;
        }
    }
    return result != 0;
}
