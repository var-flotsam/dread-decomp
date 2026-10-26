// Starts the charge: sets the rate at +0x7c (the tunable rate for the current mode times the value at
// +0x5f8, divided by the scale object's), flags the owner's objects at +0x1b8 and +0x2a0, runs
// fn_71005026d0 with the tunable's start value, clears the flag at +0x5e4 and passes the component to
// fn_7100de9540.
#include "game/UnkGameManager.h"
#include "game_s2/UnkChargeComponent.h"

extern char g_unk_71726ddc48[] __attribute__((visibility("hidden")));

void fn_7100ac3880(f32 rate);
void fn_71008618cc(void* object);
void fn_71005026d0(UnkChargeComponent* self, u32 mode, f32 value);

// NON_MATCHING(minor): the two rate addresses are computed in the other order (the select picks the same one).
void fn_7100502628(UnkChargeComponent* self) {
    const f32* second = &g_unk_71726e0aa8.field_0xac;
    const f32* first = &g_unk_71726e0aa8.field_0xa8;
    const f32* rate = self->field_0x579 == 0 ? first : second;
    self->field_0x7c = *rate * self->field_0x5f8 / self->field_0x5d0->field_0xd4;
    fn_7100ac3880(self->field_0x7c);
    if (self->field_0x30->field_0x1b8)
        self->field_0x30->field_0x1b8->field_0x64 = 1;
    if (void* object = self->field_0x30->field_0x2a0)
        fn_71008618cc(object);
    fn_71005026d0(self, 1, g_unk_71726e0aa8.field_0x9c);
    self->field_0x5e4 = 0;
    fn_7100de9540(g_unk_7101d47310, g_unk_71726ddc48, self);
}
