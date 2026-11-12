// Unit 2754 of tools/tu_map.py (no reflected class): setting the flag at +0xc and, when it turns
// on and a scene object is attached (+0x18), handing that object to the render manager's pass list
// on the layer at +0x8 (fn_7100219bb4). Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"

struct UnkSceneObj_71010e2278;

struct UnkUnit2754Item {
    u8 pad_0x0[0x8];
    u32 layer;  // 0x8
    bool enabled;  // 0xc
    u8 pad_0xd[0xb];
    UnkSceneObj_71010e2278* object;  // 0x18
};

void fn_71011f5f54(UnkUnit2754Item* self, bool enabled) {
    if (self->enabled != enabled) {
        self->enabled = enabled;
        if (enabled && self->object)
            fn_7100219bb4(g_unk_7101d47300, self->object, self->layer);
    }
}
