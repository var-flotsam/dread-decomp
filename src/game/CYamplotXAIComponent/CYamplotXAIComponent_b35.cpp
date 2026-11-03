// A function in the CYamplotXAIComponent unit: when the tunable float at +0xf4 (tunables behind +0x14728)
// is positive and fn_71006f29c4 fails for it, the 2D vector argument is scaled by 1.5. Names and layouts
// are placeholders.
#include "types.h"

struct UnkYamplotXTunables {
    u8 unk_0x0[0xf4];
    f32 field_0xf4;
};
struct UnkYamplotXAI {
    u8 unk_0x0[0x14728];
    UnkYamplotXTunables* tunables;
};
struct UnkVec2_b35a2 {
    f32 x;
    f32 y;
};

bool fn_71006f29c4(UnkYamplotXAI* self, f32 value);

void fn_71006f2974(UnkYamplotXAI* self, UnkVec2_b35a2* v) {
    f32 value = self->tunables->field_0xf4;
    if (value > 0.0f && !fn_71006f29c4(self, value)) {
        v->x = v->x * 1.5f;
        v->y = v->y * 1.5f;
    }
}
