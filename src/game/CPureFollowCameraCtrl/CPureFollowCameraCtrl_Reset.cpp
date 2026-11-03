// CPureFollowCameraCtrl unit: after the base step (fn_7100764388), stores fn_71010ee560's value at
// +0x94 and, when there is a player (fn_7100dec788 of the game manager), copies its position
// (fn_7100d7c9f4) to +0x244. Names and layouts are placeholders.
#include "game/UnkGameManager.h"

struct UnkPlayer;
struct UnkDropCameraVec3;

// The position as two words (x and y together, then z).
struct __attribute__((packed, aligned(4))) UnkPureFollowVec3 {
    u64 xy;
    u32 z;
};

struct UnkPureFollowCameraCtrl {
    u8 pad_0x0[0x94];
    f32 field_0x94;
    u8 pad_0x98[0x1ac];
    UnkPureFollowVec3 field_0x244;
};

void fn_7100764388(UnkPureFollowCameraCtrl* self);
f32 fn_71010ee560(UnkPureFollowCameraCtrl* self);
UnkPlayer* fn_7100dec788(UnkGameManager* manager);
const UnkDropCameraVec3* fn_7100d7c9f4(UnkPlayer* player);

void fn_71010f072c(UnkPureFollowCameraCtrl* self) {
    fn_7100764388(self);
    self->field_0x94 = fn_71010ee560(self);
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310)) {
        const UnkPureFollowVec3* pos = reinterpret_cast<const UnkPureFollowVec3*>(fn_7100d7c9f4(player));
        u64 xy = pos->xy;
        u32 z = pos->z;
        self->field_0x244.xy = xy;
        self->field_0x244.z = z;
    }
}
