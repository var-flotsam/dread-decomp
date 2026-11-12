// Functions in the CSynchronizedBlockCameraCtrl unit (tools/tu_map.py). Names and layouts are
// placeholders.
#include "game/CCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"

struct UnkPlayer;
struct UnkDropCameraVec3;
struct UnkChozoCommanderDropCameraCtrl;

struct UnkBlockCameraVec3 {
    u32 x;
    u32 y;
    u32 z;
};

struct UnkSynchronizedBlockCameraCtrl {
    u8 pad_0x0[0x238];
    UnkBlockCameraVec3 field_0x238;
    UnkBlockCameraVec3 field_0x244;
    u8 pad_0x250[0x4];
    u32 field_0x254;
    u8 pad_0x258[0x37];
    bool field_0x28f;
    u8 pad_0x290[0x78];
    u32 field_0x308;
    u32 field_0x30c;
    u8 pad_0x310[0x14];
    UnkBlockCameraVec3 field_0x324;
};

// As fn_7100797df0 (CChozoCommanderDropCameraCtrl): after the base step, clears the vector at
// +0x324 and, when there is a player, copies its position to +0x244 and +0x238; then copies
// +0x308 to +0x254 and clears +0x30c and the flag at +0x28f.
void fn_7100a210c8(UnkSynchronizedBlockCameraCtrl* self) {
    fn_71010ee17c(reinterpret_cast<UnkChozoCommanderDropCameraCtrl*>(self));
    self->field_0x324 = UnkBlockCameraVec3{0, 0, 0};
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310)) {
        const UnkBlockCameraVec3* pos = reinterpret_cast<const UnkBlockCameraVec3*>(fn_7100d7c9f4(player));
        u32 x = self->field_0x244.x = pos->x;
        u32 y = self->field_0x244.y = pos->y;
        u32 z = self->field_0x244.z = pos->z;
        self->field_0x238.x = x;
        self->field_0x238.y = y;
        self->field_0x238.z = z;
    }
    self->field_0x254 = self->field_0x308;
    self->field_0x30c = 0;
    self->field_0x28f = false;
}
