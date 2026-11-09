// behaviortree::CEmmyPatrolAction's unit (tools/tu_map.py unit 802): one of two times of the owner's
// (+0x18) AI component (+0x240) taken as a CEmmyAIComponent: +0x149e8 while the flag at +0x149e4 is
// set, +0x149d8 otherwise. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkEmmyPatrolAI_b0058 {
    virtual const base::reflection::CClass* vf_0x0();
    u8 pad_0x8[0x149d8 - 0x8];
    f32 field_0x149d8;
    u8 pad_0x149dc[0x149e4 - 0x149dc];
    bool field_0x149e4;
    u8 pad_0x149e5[3];
    f32 field_0x149e8;
};

struct UnkEmmyPatrolOwner_b0058 {
    u8 pad_0x0[0x240];
    UnkEmmyPatrolAI_b0058* ai;
};

struct UnkEmmyPatrolAction_b0058 {
    u8 pad_0x0[0x18];
    UnkEmmyPatrolOwner_b0058* owner;
};

f32 fn_7100514474(UnkEmmyPatrolAction_b0058* self) {
    UnkEmmyPatrolAI_b0058* ai = self->owner->ai;
    UnkEmmyPatrolAI_b0058* emmy = nullptr;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        emmy = ai;
    return emmy->field_0x149e4 ? emmy->field_0x149e8 : emmy->field_0x149d8;
}
