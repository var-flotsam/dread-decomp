// CThermalDeviceComponent's unit (tools/tu_map.py unit 1601): after the base update, moves the lit
// level of the two shared render parameters (eight floats in two vectors) from the previous index to
// the current one. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/CComponent.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU1601.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::math::CVector4D;

struct UnkThermalDevice_la9270c {
    u8 pad_0x0[0x53c];
    s32 level;     // 0x53c
    s32 previous;  // 0x540
};

// Sets one of the eight level floats and passes both vectors to the renderer.
inline void SetLevel(s32 index, f32 value) {
    switch (index) {
    case 0:
        g_unk_7172751c18.x = value;
        break;
    case 1:
        g_unk_7172751c18.y = value;
        break;
    case 2:
        g_unk_7172751c18.z = value;
        break;
    case 3:
        g_unk_7172751c18.w = value;
        break;
    case 4:
        g_unk_7101a92ec0.x = value;
        break;
    case 5:
        g_unk_7101a92ec0.y = value;
        break;
    case 6:
        g_unk_7101a92ec0.z = value;
        break;
    case 7:
        g_unk_7101a92ec0.w = value;
        break;
    default:
        return;
    }
    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164b5f0, &g_unk_7172751c18);
    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164b5f8, &g_unk_7101a92ec0);
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100a9270c(UnkThermalDevice_la9270c* self) {
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    if (self->level == self->previous) {
        SetLevel(self->level, 0.0f);
    } else {
        SetLevel(self->previous, 0.0f);
        SetLevel(self->level, self->level + 1.0f);
    }
}
