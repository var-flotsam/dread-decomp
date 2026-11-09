// behaviortree::CEmmyPatrolSearchingAction's unit (tools/tu_map.py unit 804): one of two times of the owner's
// (+0x18) AI component (+0x240) taken as a CEmmyAIComponent: +0x14a58 while the flag at +0x14a54 is
// set, +0x14a48 otherwise. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkEmmyPatrolAIS_b0058 {
    virtual const base::reflection::CClass* vf_0x0();
    u8 pad_0x8[0x14a48 - 0x8];
    f32 field_0x14a48;
    u8 pad_0x14a4c[0x14a54 - 0x14a4c];
    bool field_0x14a54;
    u8 pad_0x14a55[3];
    f32 field_0x14a58;
};

struct UnkEmmyPatrolOwnerS_b0058 {
    u8 pad_0x0[0x240];
    UnkEmmyPatrolAIS_b0058* ai;
};

struct UnkEmmyPatrolActionS_b0058 {
    u8 pad_0x0[0x18];
    UnkEmmyPatrolOwnerS_b0058* owner;
};

f32 fn_71005167cc(UnkEmmyPatrolActionS_b0058* self) {
    UnkEmmyPatrolAIS_b0058* ai = self->owner->ai;
    UnkEmmyPatrolAIS_b0058* emmy = nullptr;
    if (ai && fn_710009d50c(ai->vf_0x0(), fn_710053b934()))
        emmy = ai;
    return emmy->field_0x14a54 ? emmy->field_0x14a58 : emmy->field_0x14a48;
}
