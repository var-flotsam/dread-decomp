// CDaivoSwarmControllerComponent's unit (tools/tu_map.py): unless the flag at +0x588 is set, puts the swarm into
// state 3 through vtable slot 0x2e8. In mode 2 (+0x390) with the word at +0x530 equal to 2 it does so at once;
// otherwise only with the word at +0x2d8 set, and (unless it's in mode 1 with time left at +0x568) after
// fn_710073f7bc gets 2. Names and layouts are placeholders.
#include "game/CFlockingSwarmControllerComponent.h"
#include "types.h"

// As src/game/CRedenkiSwarmControllerComponent/CRedenkiSwarmControllerComponent.cpp declares it.
struct UnkRedenkiSwarmController;

struct UnkDaivoSwarm_A2_b68 {
    void** vtable;
    u8 pad_0x8[0x2d8 - 0x8];
    u32 field_0x2d8;
    u8 pad_0x2dc[0x390 - 0x2dc];
    u32 mode;  // 0x390
    u8 pad_0x394[0x530 - 0x394];
    u32 field_0x530;
    u8 pad_0x534[0x568 - 0x534];
    f32 field_0x568;
    u8 pad_0x56c[0x588 - 0x56c];
    bool field_0x588;

    void vf_0x2e8(u32 state, u32 arg) {
        reinterpret_cast<void (*)(UnkDaivoSwarm_A2_b68*, u32, u32)>(vtable[0x2e8 / 8])(this, state, arg);
    }
};

void fn_71004e1c90(UnkDaivoSwarm_A2_b68* self) {
    if (self->field_0x588)
        return;
    u32 mode = self->mode;
    if (!(mode == 2 && self->field_0x530 == 2)) {
        if (self->field_0x2d8 == 0)
            return;
        if (mode == 1 && !(self->field_0x568 <= 0.0f))
            return;
        fn_710073f7bc(reinterpret_cast<UnkRedenkiSwarmController*>(self), 2);
    }
    self->vf_0x2e8(3, 0);
}
