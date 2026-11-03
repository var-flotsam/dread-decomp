// Unit 39 (tools/tu_map.py): four-component colour/vector operators returned by value (through x8: the
// type has a user-written copy constructor): the component-wise sum and product, each clamped below at
// zero. Names are placeholders.
#include "types.h"
struct UnkV4_b47 {
    f32 x, y, z, w;
    UnkV4_b47() {}
    UnkV4_b47(f32 x_, f32 y_, f32 z_, f32 w_) : x(x_), y(y_), z(z_), w(w_) {}
    UnkV4_b47(const UnkV4_b47& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};
static inline f32 pos(f32 v) { return v <= 0.0f ? 0.0f : v; }
UnkV4_b47 fn_7100030770(const UnkV4_b47* a, const UnkV4_b47* b) {
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
