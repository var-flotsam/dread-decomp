// Applies a viewport (x, y, width, height, depth range): sets it on the device, stores the
// rectangle with a default 0..1 depth range in the render state, and resets related state.
#include "game/UnkTU177.h"
#include "game/UnkTU259.h"
#include "types.h"

struct UnkViewport_71001cdd74 {
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    f32 min_depth;
    f32 max_depth;
};

struct UnkState_71001cdd74 {
    s32 x;
    s32 y;
    s32 width;
    s32 height;
    f32 min_depth;
    f32 max_depth;
    u8 unk_0x18[0x2a0];
    u64 field_0x2b8;
    u8 unk_0x2c0[0x108];
    u32 field_0x3c8;
};

struct UnkStruct_71001cdd74 {
    u8 unk_0x0[0x317aa08];
    UnkState_71001cdd74 state;
};

void fn_71001cdd74(UnkStruct_71001cdd74* self, const UnkViewport_71001cdd74* vp) {
    UnkState_71001cdd74* state = &self->state;
    fn_7100159490(vp->x, vp->y, vp->width, vp->height, vp->min_depth, vp->max_depth);
    fn_7100159800(0);
    state->min_depth = 0.0f;
    state->max_depth = 1.0f;
    state->width = vp->width;
    state->height = vp->height;
    state->x = vp->x;
    state->y = vp->y;
    state->field_0x3c8 = 0;
    fn_71001cafa0(self, 0, 0, 0);
    state->field_0x2b8 = 0;
    g_unk_716e554938 = 0;
}
