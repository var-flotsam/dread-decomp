// Starts a transition toward a target (fn_7100745994) unless it already runs toward one with the same
// first word: stores the target (+0x20), the mode (+0x28) and the duration in seconds (frames / 30,
// +0x2c), and the value (+0x30), or the current one (+0x18) when the value is negative. The unit has
// no class name. Names and layouts are placeholders.
#include "types.h"

struct UnkTransitionTarget {
    u64 field_0x0;
};

struct UnkTransition {
    u8 pad_0x0[0x18];
    f32 current;
    u8 pad_0x1c[4];
    UnkTransitionTarget* target;
    u32 mode;
    f32 duration;
    f32 value;
};

void fn_7100745994(UnkTransition* self, UnkTransitionTarget* target, u32 frames, u32 mode, f32 value) {
    if (target && self->target && self->target->field_0x0 == target->field_0x0)
        return;
    self->target = target;
    self->mode = mode;
    self->duration = (f32)frames * 0.03333333f;
    if (value >= 0.0f)
        self->value = value;
    else
        __builtin_memcpy(&self->value, &self->current, sizeof(f32));
}
