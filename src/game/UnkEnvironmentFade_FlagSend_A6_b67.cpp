// Unit 2607 (tools/tu_map.py), the environment's fading render settings (src/game/UnkEnvironmentFade.cpp):
// stores a flag and sends it, with the word at +0x280, to the render manager's pass parameter
// 0xbd88e4a69dc0e5c9 (fn_7100219294). Names and layout are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkEnvFadeFlag_A6_b67 {
    u8 pad_0x0[0x271];
    bool field_0x271;
    u8 pad_0x272[0xe];
    u64 field_0x280;
};

void fn_7100f98998(UnkEnvFadeFlag_A6_b67* self, bool flag) {
    self->field_0x271 = flag;
    u64 value[2];
    value[0] = flag;
    value[1] = self->field_0x280;
    fn_7100219294(g_unk_7101d47300, 0xbd88e4a69dc0e5c9, reinterpret_cast<const f32*>(value), 0);
}
