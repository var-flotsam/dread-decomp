// CAutsniperShootAttack's unit (tools/tu_map.py): advances the timer at 0x94 by `dt`; once it reaches the
// limit at 0x100 of the object at 0x28 it is cleared, the entity's sub-object at +0x220 is sent a zero
// (refl_EmitU32) and, unless the state at 0x50 is already 1, the attack switches to state 1
// (fn_71003d3cf8). Names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkAutsniperSendEntity_b58 {
    u8 unk_0x0[0x220];
    void* field_0x220;
};
struct UnkAutsniperSendLimits_b58 {
    u8 unk_0x0[0x100];
    f32 limit;
};
struct UnkAutsniperSend_b58 {
    u8 unk_0x0[8];
    UnkAutsniperSendEntity_b58* entity;
    u8 unk_0x10[0x18];
    UnkAutsniperSendLimits_b58* limits;
    u8 unk_0x30[0x20];
    s32 state;
    u8 unk_0x54[0x40];
    f32 timer;
};

void fn_71003d3cf8(UnkAutsniperSend_b58* self);

void fn_71003d6a84(UnkAutsniperSend_b58* self, f32 dt) {
    f32 timer = self->timer + dt;
    self->timer = timer;
    if (!(timer >= self->limits->limit))
        return;
    self->timer = 0.0f;
    base::reflection::refl_EmitU32(self->entity->field_0x220, 0x46cb3337bfed7326, 0x71b15e29dc542a11, 0,
                                   &g_unk_710164b958, &g_unk_7172769740);
    if (self->state != 1) {
        self->state = 1;
        fn_71003d3cf8(self);
    }
}
