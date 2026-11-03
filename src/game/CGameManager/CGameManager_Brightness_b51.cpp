// CGameManager unit (tools/tu_map.py, the tunable names from reflection): sets render pass parameter
// 0x20287476e340c4fb (fn_7100219294) to a four-float value derived from `level` (0-10 maps to 0.0-1.0 in
// steps of 0.1, higher levels to 1.0 plus a tenth of the excess over 10); negative levels are ignored.
// Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

void fn_7100df9358(void* self, u32 index, s32 level) {
    if (level < 0)
        return;
    f32 v = level < 11 ? (f32)level * 0.1f + 0.0f : (f32)(level - 10) * 0.1f + 1.0f;
    f32 value[4] = {v, v, v, v};
    fn_7100219294(g_unk_7101d47300, 0x20287476e340c4fb, value, index);
}
