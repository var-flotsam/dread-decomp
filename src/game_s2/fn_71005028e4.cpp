// Per-frame step of the charge: sets the rate at +0x7c (as fn_7100502628 does), runs the two update
// helpers, then moves the charge at +0x5dc toward the tunable's limit by 100 per second (via
// fn_71005026d0, capped at the limit). Returns the limit when the charge is already there.
#include "game_s2/UnkChargeComponent.h"

void fn_7100ac38fc(f32 rate);
void fn_7100ac40fc(UnkChargeComponent* self, f32 time);
f32 fn_71005026d0(UnkChargeComponent* self, u32 mode, f32 value);

// NON_MATCHING(minor): the two rate addresses are computed in the other order (the select picks the same one).
f32 fn_71005028e4(UnkChargeComponent* self, f32 time) {
    const f32* rate = self->field_0x579 == 0 ? &g_unk_71726e0aa8.field_0xa8 : &g_unk_71726e0aa8.field_0xac;
    self->field_0x7c = *rate * self->field_0x5f8 / self->field_0x5d0->field_0xd4;
    fn_7100ac38fc(self->field_0x7c);
    fn_7100ac40fc(self, time);
    f32 current = self->field_0x5dc;
    f32 limit = g_unk_71726e0aa8.field_0xa0;
    if (current != limit) {
        f32 next = time * 100.0f + current;
        return fn_71005026d0(self, 0, next > limit ? limit : next);
    }
    return limit;
}
