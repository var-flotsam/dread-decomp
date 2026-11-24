// Unit 39 (tools/tu_map.py): four-component colour/vector operators returned by value (through x8: the
// type has a user-written copy constructor): the component-wise sum and product, each clamped below at
// zero. Names are placeholders.
#include "game/UnkTU39.h"
#include "game/UnkV4_b47.h"
#include "types.h"
static inline f32 pos(f32 v) { return v <= 0.0f ? 0.0f : v; }
UnkV4_b47 AddVec4ClampedNonNegative(const UnkV4_b47* a, const UnkV4_b47* b) {
    UnkV4_b47 s(a->x + b->x, a->y + b->y, a->z + b->z, a->w + b->w);
    UnkV4_b47 r;
    r.x = pos(s.x);
    r.y = pos(s.y);
    r.z = pos(s.z);
    r.w = pos(s.w);
    return r;
}
UnkV4_b47 fn_71000307d0(const UnkV4_b47* a, const UnkV4_b47* b) {
    UnkV4_b47 s(a->x * b->x, a->y * b->y, a->z * b->z, a->w * b->w);
    UnkV4_b47 r;
    r.x = pos(s.x);
    r.y = pos(s.y);
    r.z = pos(s.z);
    r.w = pos(s.w);
    return r;
}
