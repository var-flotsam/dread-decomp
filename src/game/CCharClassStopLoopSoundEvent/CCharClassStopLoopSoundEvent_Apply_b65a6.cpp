// CCharClassStopLoopSoundEvent's unit: when the target is set, passes the event's name (+0x10) and two
// values (+0x18, +0x1c) to fn_7100d7c48c. Names are placeholders.
#include "game/UnkTU2523.h"
#include "types.h"

struct UnkStopLoopSoundEvent_b65a6 {
    u8 pad_0x0[0x10];
    u8 name[8];
    f32 value_0x18;
    f32 value_0x1c;
};

void fn_7100ce39cc(UnkStopLoopSoundEvent_b65a6* self, void** target) {
    if (void* t = *target)
        fn_7100d7c48c(t, self->name, self->value_0x18, self->value_0x1c);
}
