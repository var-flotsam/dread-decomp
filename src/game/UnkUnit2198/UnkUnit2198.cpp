// From unit 2198 of tools/tu_map.py (0x7100c80924-0x7100c81264, no reflected class): a sample handler (see
// src/game_aca57c/fn_7100c79a80.cpp, whose declaration this copies) that sets a render pass parameter: the
// hash is the first word of the object at +0x10 of the parameters, the value pointer their first word, with
// a count of 0. Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkSampleParams;

struct UnkPassParamHash {
    u64 hash;
};

struct UnkPassParamSetter {
    const f32* value;
    u8 pad_0x8[0x8];
    UnkPassParamHash* name;
};

void fn_7100c80924(UnkSampleParams* params) {
    UnkPassParamSetter* self = (UnkPassParamSetter*)params;
    fn_7100219294(g_unk_7101d47300, self->name->hash, self->value, 0);
}
