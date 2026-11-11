// CNailongAIComponent's unit (tools/tu_map.py), fn_710062175c: after the base step (fn_7101108124), when the
// direction to the given actor, scaled on x by the speed (+0x148dc), points backwards outside +-0.01, the speed
// is negated and, if the side (+0x1483c: 1 while the old speed was negative) changes, ids 4/8 are set and cleared
// on the controller (+0x220) as CShakernautAIComponent_Flip_b53.cpp sends them. Names and layouts are placeholders.
#include <math.h>
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU2702.h"
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};
// As src/game/CQuetzoaAIComponent/CQuetzoaAIComponent_b38.cpp declares it.
struct UnkQuetzoaAIComponent;

namespace {
struct V3 {
    f32 x, y, z;
};
inline V3 operator-(V3 a, V3 b) { return V3{a.x - b.x, a.y - b.y, a.z - b.z}; }
}  // namespace

struct UnkNailongTurnState_A3_b76 {
    u32 side;  // 0x1483c
    u8 pad_0x4[0xa0 - 0x4];
    f32 speed;  // 0x148dc
};

struct UnkNailongTurnAI_A3_b76 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x1483c - 0x38];
    UnkNailongTurnState_A3_b76 state;  // 0x1483c
};

void fn_710062175c(UnkNailongTurnAI_A3_b76* self, UnkActor* target) {
    UnkNailongTurnState_A3_b76* st = &self->state;
    fn_7101108124(reinterpret_cast<UnkQuetzoaAIComponent*>(self), target);
    V3 dir = V3{target->field_0x44, target->field_0x54, target->field_0x64} -
             V3{self->owner->field_0x44, self->owner->field_0x54, self->owner->field_0x64};
    f32 inv = 1.0f / (sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z) + 0.0001f);
    V3 n = V3{dir.x * inv, dir.y * inv, dir.z * inv};
    f32 speed = st->speed;
    f32 d = speed * n.x + 0.0f * n.y + 0.0f * n.z;
    if (d < 0.0f && !(d + 0.01f >= 0.0f && d - 0.01f <= 0.0f)) {
        bool neg = speed < 0.0f;
        u32 side = st->side;
        st->speed = -speed;
        if (side != neg) {
            UnkShakernautId_b53 a1, b1, a2, b2;
            UnkShakernautCtrl_b53* ctrl = static_cast<UnkShakernautCtrl_b53*>(self->owner->field_0x220);
            st->side = neg;
            UnkShakernautId_b53* a = neg ? &a1 : &a2;
            a->value = neg ? 4 : 8;
            UnkShakernautId_b53* b = neg ? &b1 : &b2;
            u32 bv = neg ? 8 : 4;
            fn_71011785a8(ctrl, a, true, false);
            b->value = bv;
            fn_7101178564(ctrl, b, true, true);
        }
    }
}
