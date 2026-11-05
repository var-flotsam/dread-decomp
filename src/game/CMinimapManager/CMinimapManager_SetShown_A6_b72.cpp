// CMinimapManager unit (tools/tu_map.py): stores a flag at +0xd63d6b, passes it to the object at +0xd64548
// (fn_7100eb7358), then copies that object's float at +8 into a unit global and hands it to the render manager's
// pass parameter 0xe936254db735c615 (mode 2); the float is returned. Names and layouts are placeholders.
#include "../../game_s7/UnkGuiS7.h"
#include "game/UnkRenderManager.h"
#include "types.h"

extern f32 g_unk_71729cdb4c __attribute__((visibility("hidden")));

f32 fn_7100e943a0(char* self, bool flag) {
    *reinterpret_cast<bool*>(self + 0xd63d6b) = flag;
    unk_gui::fn_7100eb7358(self + 0xd64548, flag, 0);
    asm("" : "+r"(self));
    f32 v = *reinterpret_cast<f32*>(self + 0xd64550);
    g_unk_71729cdb4c = v;
    fn_7100219294(g_unk_7101d47300, 0xe936254db735c615, &g_unk_71729cdb4c, 2);
    return v;
}
