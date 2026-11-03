// CObsydomithonAttack unit (tools/tu_map.py): whether the value from fn_7100629368 lies strictly
// between -0.001 and the limit at +0xec of the object at +0x28. Names and layouts are placeholders.
#include "types.h"

struct UnkObsydomithonParams {
    u8 pad_0x0[0xec];
    f32 limit;  // 0xec
};

struct UnkObsydomithonAttack {
    u8 pad_0x0[0x28];
    UnkObsydomithonParams* params;  // 0x28
};

f32 fn_7100629368(UnkObsydomithonAttack* self);

bool fn_710062a0c8(UnkObsydomithonAttack* self) {
    f32 v = fn_7100629368(self);
    if (v > -0.001f && v < self->params->limit)
        return true;
    return false;
}
