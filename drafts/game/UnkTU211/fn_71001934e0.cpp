// UnkTU211: reads one of two 16-byte colour records of an indexed entry into a colour; false when the
// selector names neither (the colour is then zero). Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "types.h"

using base::math::CVector4D;
using base::math::UnkVector4D;

struct UnkColorEntry {
    u8 pad_0x0[0x14c];
    UnkVector4D colorsB_0x14c[1];
    u8 pad_0x15c[0x240 - 0x15c - 0x0];
    UnkVector4D colorsA_0x240[1];
};

struct UnkColorOwner {
    u8 pad_0x0[0x40];
    UnkColorEntry** entries_0x40;
};

void fn_7100030938(base::color::CColor4F* self, u32 mode, f32 r, f32 g, f32 b, f32 a);

// Assigns a copy of the colour (the record is copied before the assignment).
static inline void AssignCopy(CVector4D& dest, UnkVector4D value) {
    Assign(dest, value);
}

bool fn_71001934e0(UnkColorOwner* self, u32 entryIndex, u32 colorIndex, base::color::CColor4F* out,
                   u32 selector) {
    CVector4D color = {0.0f, 0.0f, 0.0f, 0.0f};
    UnkColorEntry* entry = self->entries_0x40[entryIndex];
    bool found;
    if (selector == 0) {
        AssignCopy(color, entry->colorsA_0x240[colorIndex]);
        found = true;
    } else if (selector == 1) {
        AssignCopy(color, entry->colorsB_0x14c[colorIndex]);
        found = true;
    } else {
        found = false;
    }
    fn_7100030938(out, 0, color.x, color.y, color.z, color.w);
    return found;
}
