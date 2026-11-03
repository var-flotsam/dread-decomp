// CHydrogigaAIComponent (the name from reflection, tools/tu_map.py): whether the AI may act now.
// Once its timer at +0x149c8 has reached the tunable at +0x554 of its parameters (+0x149a0), it
// may when its target has no helper at +0x208, or the helper is neither busy (fn_71009e6f9c) nor
// fn_71009e6434. Names and layout are placeholders.
#include "game/UnkActor.h"
#include "types.h"

struct UnkHelper;
bool fn_71009e6434(UnkHelper* helper);
bool fn_71009e6f9c(UnkHelper* helper);

struct UnkHydrogigaParams_b44a2 {
    u8 pad_0x0[0x554];
    f32 field_0x554;
};

struct UnkHydrogigaAI_b44a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x149a0 - 0x38];
    UnkHydrogigaParams_b44a2* params;
    u8 pad_0x149a8[0x20];
    f32 field_0x149c8;
};

bool fn_71005c01ac(UnkHydrogigaAI_b44a2* self) {
    if (!(self->field_0x149c8 >= self->params->field_0x554))
        return false;
    UnkActor* target = fn_7100d7c990(self->owner);
    if (!target)
        return true;
    auto* helper = static_cast<UnkHelper*>(target->field_0x208);
    if (!helper)
        return true;
    if (fn_71009e6f9c(helper))
        return false;
    return !fn_71009e6434(helper);
}
