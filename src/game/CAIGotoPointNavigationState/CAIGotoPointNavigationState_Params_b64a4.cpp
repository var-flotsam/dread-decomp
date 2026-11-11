// CAIGotoPointNavigationState unit (tools/tu_map.py): the parameter-block handler. After the shared
// step (fn_71007038f4), blocks of kind 0 or 1 go to the member at +0xb0 (fn_710037c884); the flag at
// +0x38180 is raised on a reset or by a kind-1 block whose optional target (+0x60, set flag +0x68) is
// present. Names and layouts are placeholders.
#include "game/UnkTU1124.h"
#include "types.h"

struct UnkGotoParams_b64a4 {
    u8 pad_0x0[0x34];
    u32 kind;
    u8 pad_0x38[0x28];
    void* target;  // 0x60
    bool has_target;
};

struct UnkGotoState_b64a4 {
    u8 pad_0x0[0x38180];
    bool field_0x38180;
};

void fn_710037c884(void* member, UnkGotoParams_b64a4* params, bool reset);

void fn_7100702618(UnkGotoState_b64a4* self, UnkGotoParams_b64a4* params, bool reset) {
    fn_71007038f4(self, params, reset);
    if (params->kind > 1)
        return;
    fn_710037c884(reinterpret_cast<u8*>(self) + 0xb0, params, reset);
    if (reset || (params->kind == 1 && params->has_target && params->target))
        self->field_0x38180 = true;
}
