// behaviortree::CRinkaRandomShootAction's unit (tools/tu_map.py): picks one of three shots by weight (the
// weights at +0x30, +0x34 and +0x38; a draw in [0, 1] scaled by their sum, or used as it is when the sum
// is not positive: below 0.33, below 0.66, else the third) and hands the pick, with the action's slot
// (+0x2c), to the owner's central-unit AI component (fn_7100405d58). A negative slot fails (6); a weight
// that is zero is never picked. Names and layouts are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkAIComponent.h"
#include "types.h"

extern "C" int rand();

// Within 0.001 of b.
#define UNK_NEAR(a, b) ((a) - 0.001f <= (b) && (a) + 0.001f >= (b))

using base::reflection::UnkReflObject;

struct UnkRinkaOwner_ovb0267 {
    u8 pad_0x0[0x240];
    UnkReflObject* field_0x240;
};

struct UnkRinkaShoot_ovb0267 {
    u8 pad_0x0[0x18];
    UnkRinkaOwner_ovb0267* field_0x18;
    u8 pad_0x20[0xc];
    s32 field_0x2c;
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
};


void fn_7100405d58(UnkAIComponent*, s32, u32);

u32 fn_7100402714(UnkRinkaShoot_ovb0267* self) {
    if (self->field_0x2c < 0)
        return 6;
    f32 total = self->field_0x30 + self->field_0x34 + self->field_0x38;
    f32 draw = (f32)((rand() & 0x7fff) * (1.0 / 32767.0));
    u32 pick;
    if (total <= 0.0f) {
        f32 v = draw + 0.0f;
        pick = v < 0.33f ? 0 : (!(v < 0.66f) ? 2 : 1);
    } else {
        f32 v = total * draw + 0.0f;
        if (!UNK_NEAR(self->field_0x30, 0.0f) && v <= self->field_0x30) {
            pick = 0;
        } else if (!UNK_NEAR(self->field_0x34, 0.0f) && v <= self->field_0x30 + self->field_0x34) {
            pick = 1;
        } else {
            pick = 2;
            if (self->field_0x38 + 0.001f >= 0.0f && self->field_0x38 - 0.001f <= 0.0f)
                return 5;
        }
    }
    UnkReflObject* comp = self->field_0x18->field_0x240;
    if (comp && fn_710009d50c(comp->vf_0x0(), fn_71004030f4()))
        fn_7100405d58(reinterpret_cast<UnkAIComponent*>(comp), self->field_0x2c, pick);
    return 5;
}
