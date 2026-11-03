// Unit 247: sets colour `index` of one of two sets (kind 0: colours at +0x240, targets at +0x2e0;
// kind 1: +0x14c and +0x1f0) in the shared state (+0x88) from a CColor4F, and the bound
// target's vector (its +0x30) when there is one, as fn_71001bfea4 does. Names and layouts are
// placeholders.
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "types.h"

using base::math::CVector4D;

struct UnkColorTarget247 {
    u8 pad_0x0[0x30];
    CVector4D* color;  // 0x30
};

struct UnkColorOwner247 {
    u8 pad_0x0[0x88];
    char* state;
};

template <u32 Colors, u32 Targets>
__attribute__((always_inline)) inline void SetStateColor(UnkColorOwner247* self, u32 index,
                                                         const base::color::CColor4F* color) {
    char* state = self->state;
    base::math::UnkVector4D v = fn_7100030878(color);
    fn_710007b518(reinterpret_cast<CVector4D*>(state + Colors) + index, &v);
    UnkColorTarget247* target = reinterpret_cast<UnkColorTarget247**>(state + Targets)[index];
    if (target)
        *target->color = v;
}

void fn_71001bc150(UnkColorOwner247* self, u32 index, const base::color::CColor4F* color, u32 kind) {
    switch (kind) {
    case 0:
        SetStateColor<0x240, 0x2e0>(self, index, color);
        break;
    case 1:
        SetStateColor<0x14c, 0x1f0>(self, index, color);
        break;
    }
}
