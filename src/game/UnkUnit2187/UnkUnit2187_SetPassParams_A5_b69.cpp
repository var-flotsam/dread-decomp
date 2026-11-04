// Unit 2187 (no reflection hint): sets two render pass parameters (by hash) from the pair of floats
// the object's first member points to. Names and layouts are placeholders.
#include "game/UnkRenderManager.h"

struct UnkPassParams_A5_b69 {
    f32 first;
    f32 second;
};

struct UnkPassParamsOwner_A5_b69 {
    UnkPassParams_A5_b69* params;
};

void fn_7100c7b9bc(UnkPassParamsOwner_A5_b69* self) {
    f32 first = self->params->first;
    f32 second = self->params->second;
    fn_7100219294(g_unk_7101d47300, 0xd8cb39e0e66e0d64, &first, 0);
    fn_7100219294(g_unk_7101d47300, 0x9daa3a4588a370dc, &second, 0);
}
