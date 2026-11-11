// CShakernautAIComponent's unit (tools/tu_map.py, the name from reflection): flips the component's
// orientation state at +0x147f8: after fn_710110ca7c, takes a value from its owner (+0x724), negates
// the direction pair (+0x18, +0x1c), toggles the side flag (+0xc) and sends the two matching ids
// (4 and 8, swapped by side) to the actor's +0x220 controller (fn_71011785a8, fn_7101178564), then
// clears a byte (+0xab). Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "types.h"

struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};
void fn_710110ca7c(void* self);

struct UnkShakernautDir_b53 {
    f32 x;
    f32 y;
    UnkShakernautDir_b53 operator-() const { return {-x, -y}; }
};

struct UnkShakernautOwner_b53 {
    u8 unk_0x0[0x724];
    u32 field_0x724;
};

struct UnkShakernautSide_b53 {
    UnkShakernautOwner_b53* owner;
    u8 unk_0x8[4];
    u32 side;  // 0xc
    u8 unk_0x10[4];
    u32 field_0x14;
    UnkShakernautDir_b53 dir;  // 0x18
    u8 unk_0x20[0x8b];
    bool field_0xab;
};

struct UnkShakernautActor_b53 {
    u8 unk_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;
};

struct UnkShakernautAIComponent_b53 {
    u8 unk_0x0[0x30];
    UnkShakernautActor_b53* actor;
    u8 unk_0x38[0x147f8 - 0x38];
    UnkShakernautSide_b53 state;
};

void fn_71006b0d44(UnkShakernautAIComponent_b53* self) {
    UnkShakernautSide_b53* s = &self->state;
    fn_710110ca7c(self);
    s->field_0x14 = s->owner->field_0x724;
    s->dir = -s->dir;
    UnkShakernautCtrl_b53* ctrl = self->actor->ctrl;
    if (s->side == 0) {
        s->side = 1;
        UnkShakernautId_b53 a;
        a.value = 4;
        fn_71011785a8(ctrl, &a, true, false);
        UnkShakernautId_b53 b;
        b.value = 8;
        fn_7101178564(ctrl, &b, true, false);
    } else {
        s->side = 0;
        UnkShakernautId_b53 a;
        a.value = 8;
        fn_71011785a8(ctrl, &a, true, false);
        UnkShakernautId_b53 b;
        b.value = 4;
        fn_7101178564(ctrl, &b, true, false);
    }
    s->field_0xab = false;
}
