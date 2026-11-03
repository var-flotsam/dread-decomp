// tools/tu_map.py unit 110: the squared length (and a relative distance) of a 2D segment whose length is cached at +0x10
// (negative until first computed). Names are placeholders.
#include <math.h>

#include "types.h"

struct UnkCachedSegment2D {
    f32 x0, y0;  // 0x0
    f32 x1, y1;  // 0x8
    f32 length;  // 0x10, < 0 when not computed yet
};

static inline f32 CachedLength(UnkCachedSegment2D* self) {
    if (self->length < 0.0f) {
        f32 dx = self->x1 - self->x0;
        f32 dy = self->y1 - self->y0;
        self->length = sqrtf(dx * dx + dy * dy);
    }
    return self->length;
}

f32 fn_7100126238(UnkCachedSegment2D* self) {
    f32 len = CachedLength(self);
    return len * len;
}

struct UnkVec2fP {
    f32 x, y;
};

// The distance from the start to a point as a fraction of the length, clamped to [0, 1].
f32 fn_7100125f00(UnkCachedSegment2D* self, const UnkVec2fP* p) {
    f32 dx = p->x - self->x0;
    f32 dy = p->y - self->y0;
    f32 d = sqrtf(dx * dx + dy * dy);
    f32 t = d / CachedLength(self);
    return t <= 0.0f ? 0.0f : (1.0f < t ? 1.0f : t);
}
