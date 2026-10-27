// Fetches the colour of a slot of the slot table (game/UnkSlots.h, at +0x78), clamps each
// component to be non-negative and stores it through the colour's assignment, fn_7100030728
// (which clamps again).
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "game/UnkSlots.h"
#include "types.h"

using base::color::CColor4F;
using base::math::CVector4D;

static inline f32 UnkClampPositive(f32 x) { return x <= 0.0f ? 0.0f : x; }

bool fn_71001c287c(UnkSlots* self, u32 a, u32 b, CColor4F* out, u32 mode) {
    CVector4D box = {};
    bool r = fn_71001997c8(self->info, a, b, &box, mode);
    CColor4F tmp;
    tmp.r = UnkClampPositive(box.x);
    tmp.g = UnkClampPositive(box.y);
    tmp.b = UnkClampPositive(box.z);
    tmp.a = UnkClampPositive(box.w);
    fn_7100030728(out, &tmp);
    return r;
}
