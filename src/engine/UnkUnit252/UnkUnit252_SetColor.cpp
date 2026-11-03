// Unit 252 of tools/tu_map.py (0x71001bdeac-0x71001c071c): setting one of an object's colours
// (+0x74, 16 bytes each) from a CColor4F, mirroring it into its shared state (+0xe8, colours at
// +0x14c) and into the bound target's vector (+0x1f0 per index, its +0x30) when there is one. Names
// and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector4D;

struct UnkColorTarget252 {
    u8 pad_0x0[0x30];
    CVector4D* color;  // 0x30
};

struct UnkColorOwner252 {
    u8 pad_0x0[0x74];
    CVector4D colors[7];  // 0x74
    u8 pad_0xe4[0x4];
    char* state;  // 0xe8: colours at +0x14c, targets at +0x1f0 (one per index)
};
static_assert(__builtin_offsetof(UnkColorOwner252, state) == 0xe8, "UnkColorOwner252 layout");

void fn_71001bfea4(UnkColorOwner252* self, u32 index, const base::color::CColor4F* color) {
    base::math::UnkVector4D v = fn_7100030878(color);
    CVector4D* own = &self->colors[index];
    fn_710007b518(own, &v);
    char* state = self->state;
    fn_710007b518(reinterpret_cast<CVector4D*>(state + 0x14c) + index, own);
    UnkColorTarget252* target = reinterpret_cast<UnkColorTarget252**>(state + 0x1f0)[index];
    if (target)
        *target->color = *own;
}
