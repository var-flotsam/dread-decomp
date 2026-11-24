// malloc unit: the unit direction of a segment's vector, or zero when its two points coincide.
// Names and layouts are placeholders.
#include <math.h>

#include "base/math/UnkSegment2D.h"

extern const f32 g_unk_7101647774;

struct UnkDirectionSource {
    f32 ax;
    f32 ay;
    f32 bx;
    f32 by;
    f32 dirX;
    f32 dirY;
};

UnkVec2Result fn_7100125a64(const UnkDirectionSource* s) {
    if (s->ax - g_unk_7101647774 <= s->bx && s->ax + g_unk_7101647774 >= s->bx) {
        if (s->ay - g_unk_7101647774 <= s->by && s->ay + g_unk_7101647774 >= s->by) {
            return UnkVec2Result(0.0f, 0.0f);
        }
    }
    f32 inv = 1.0f / sqrtf(s->dirX * s->dirX + s->dirY * s->dirY);
    return UnkVec2Result(inv * s->dirX, inv * s->dirY);
}
