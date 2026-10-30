// A 2D segment's length, computed on first use and cached in field_0x10 (negative until then).
#include <math.h>

#include "base/math/UnkSegment2D.h"

f32 fn_7100125fbc(const UnkSegment2D* seg) {
    UnkSegment2D* self = const_cast<UnkSegment2D*>(seg);
    if (self->field_0x10 < 0.0f) {
        f32 dx = self->end.x - self->start.x;
        f32 dy = self->end.y - self->start.y;
        self->field_0x10 = sqrtf(dx * dx + dy * dy);
    }
    return self->field_0x10;
}
