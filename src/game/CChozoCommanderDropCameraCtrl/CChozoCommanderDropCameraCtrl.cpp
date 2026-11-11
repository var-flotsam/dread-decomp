// In the CChozoCommanderDropCameraCtrl unit: after the base step (fn_71010ee17c), clears the vector at
// +0x2a4 and, when there is a player (fn_7100dec788 of the game manager), copies its position
// (fn_7100d7c9f4) to +0x244 and +0x238; then clears +0x254. Names and layouts are placeholders.
#include "game/CCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"

struct UnkPlayer;

struct UnkDropCameraVec3 {
    u32 x;
    u32 y;
    u32 z;
};

struct UnkChozoCommanderDropCameraCtrl {
    u8 pad_0x0[0x238];
    UnkDropCameraVec3 field_0x238;
    UnkDropCameraVec3 field_0x244;
    u8 pad_0x250[0x4];
    u32 field_0x254;
    u8 pad_0x258[0x4c];
    UnkDropCameraVec3 field_0x2a4;
};

UnkPlayer* fn_7100dec788(UnkGameManager* manager);

void fn_7100797df0(UnkChozoCommanderDropCameraCtrl* self) {
    fn_71010ee17c(self);
    self->field_0x2a4 = UnkDropCameraVec3{0, 0, 0};
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310)) {
        const UnkDropCameraVec3* pos = fn_7100d7c9f4(player);
        u32 x = self->field_0x244.x = pos->x;
        u32 y = self->field_0x244.y = pos->y;
        u32 z = self->field_0x244.z = pos->z;
        self->field_0x238.x = x;
        self->field_0x238.y = y;
        self->field_0x238.z = z;
    }
    self->field_0x254 = 0;
}
