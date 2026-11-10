// CPureFollowCameraCtrl unit (tools/tu_map.py): snaps the followed position (+0x238) to its target
// (+0x244) and resets the two limits at +0x254 to 1200. Names and layouts are placeholders.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkPureFollowCameraCtrl_ovb0001 {
    u8 pad_0x0[0x238];
    UnkVec3f position;  // 0x238
    UnkVec3f target;  // 0x244
    u8 pad_0x250[0x4];
    f32 field_0x254;
    f32 field_0x258;
};

void fn_71010f08b0(UnkPureFollowCameraCtrl_ovb0001* self) {
    self->position = self->target;
    self->field_0x254 = 1200.0f;
    self->field_0x258 = 1200.0f;
}
