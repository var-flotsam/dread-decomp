// Unit 2530 (tools/tu_map.py, no name hint): fills four parameters for preset 0 or 1 (zeros for any
// other preset). Names and layout are placeholders.
#include "types.h"

struct UnkPresetParams_b48a3 {
    f32 a;
    f32 b;
    f32 c;
    f32 d;
};

void fn_7100da28a4(void* self, UnkPresetParams_b48a3* out, s32 preset) {
    switch (preset) {
    case 0:
        out->c = 0.1f;
        out->d = 0.07f;
        out->a = 25.0f;
        out->b = 0.08f;
        break;
    case 1:
        out->c = 0.13f;
        out->d = 0.02f;
        out->a = 25.0f;
        out->b = 0.014f;
        break;
    default:
        out->a = 0.0f;
        out->b = 0.0f;
        out->c = 0.0f;
        out->d = 0.0f;
        break;
    }
}
