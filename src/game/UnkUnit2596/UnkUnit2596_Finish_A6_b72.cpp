// Unit 2596 (tools/tu_map.py): runs fn_7100f17714 on the object, then fn_7100f00714 on the scene state at
// g_unk_7101d47320 (created on first use, as in src/game/UnkLoadingScreen.cpp), then the object's callback at
// +0x48 if it has one. Names and layouts are placeholders.
#include "types.h"

#include "base/UnkVector.h"
struct UnkStruct_0x128;
extern UnkStruct_0x128* g_unk_7101d47320;
void fn_7100ef3494(UnkStruct_0x128* self);
void fn_7100f00714(UnkStruct_0x128* self);

struct UnkUnit2596_A6_b72 {
    u8 pad_0x0[0x48];
    void (*callback)();
};

void fn_7100f17714(UnkUnit2596_A6_b72* self);

static inline UnkStruct_0x128* SceneState_A6_b72() {
    UnkStruct_0x128* s = g_unk_7101d47320;
    if (!s) {
        s = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(s);
        g_unk_7101d47320 = s;
    }
    return s;
}

void fn_7100f17a5c(UnkUnit2596_A6_b72* self) {
    fn_7100f17714(self);
    fn_7100f00714(SceneState_A6_b72());
    if (self->callback)
        self->callback();
}
