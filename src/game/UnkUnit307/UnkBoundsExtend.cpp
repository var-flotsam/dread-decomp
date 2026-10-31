// From unit 307 of tools/tu_map.py (0x7100215dac-0x710021685c, no reflected class): grows a box
// (three min/max ranges at +0x8) to contain a point. A range whose minimum is not at most its maximum is
// empty and becomes the point. Called by fn_7100212040 (src/game_s1/fn_7100212040.cpp), which
// passes its own second argument through. Names and layouts are placeholders.
#include "types.h"

struct UnkRange {
    f32 min;
    f32 max;
};

struct UnkBox {
    u8 pad_0x0[0x8];
    UnkRange x;
    UnkRange y;
    UnkRange z;
};

struct UnkPoint3 {
    f32 x, y, z;
};

static inline void ExtendRange(UnkRange& r, f32 v) {
    if (!(r.min <= r.max)) {
        r.min = v;
        r.max = v;
    } else if (r.min > v) {
        r.min = v;
    } else if (r.max < v) {
        r.max = v;
    }
}

void fn_7100215f60(UnkBox* self, const UnkPoint3& point) {
    ExtendRange(self->x, point.x);
    ExtendRange(self->y, point.y);
    ExtendRange(self->z, point.z);
}
