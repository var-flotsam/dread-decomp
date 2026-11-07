// CDrop (tools/tu_map.py; the name from reflection): an overload of the drop spawn
// (fn_71007c02dc) that passes a copy of the position and a zero vector. Names and layout are
// placeholders.
#include "types.h"

// Copied member by member (the original loads and stores each component).
struct UnkDropVec3_b44a2 {
    f32 x, y, z;
    UnkDropVec3_b44a2(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkDropVec3_b44a2(const UnkDropVec3_b44a2& o) : x(o.x), y(o.y), z(o.z) {}
};

void fn_71007c02dc(void* a, void* b, const UnkDropVec3_b44a2* pos, const UnkDropVec3_b44a2* velocity, void* d,
                   bool e, bool f, f32 g, f32 h);

// `g` and `h` (s0, s1) reach the spawn untouched in the original (tools/forwarded_args.py).
void fn_71007c0284(void* a, void* b, const UnkDropVec3_b44a2* pos, void* d, bool e, bool f, f32 g, f32 h) {
    UnkDropVec3_b44a2 p = *pos;
    UnkDropVec3_b44a2 v(0.0f, 0.0f, 0.0f);
    fn_71007c02dc(a, b, &p, &v, d, e, f, g, h);
}
