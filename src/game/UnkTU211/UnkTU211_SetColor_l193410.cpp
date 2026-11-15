// UnkTU211: sets one of two colour sets (selector 0 or 1) of an owner from a vector, mirroring it into the
// indexed entry's colours and into the vector of the target bound there. Names and layouts are
// placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkTU211.h"
#include "types.h"

using base::math::CVector4D;

struct UnkColorTarget {
    u8 pad_0x0[0x30];
    CVector4D* color_0x30;
};

struct UnkColorEntry {
    u8 pad_0x0[0x14c];
    CVector4D colorsB_0x14c[10];
    UnkColorTarget* targetsB_0x1f0[10];
    CVector4D colorsA_0x240[10];
    UnkColorTarget* targetsA_0x2e0[10];
};
static_assert(__builtin_offsetof(UnkColorEntry, targetsA_0x2e0) == 0x2e0, "UnkColorEntry layout");

struct UnkColorOwner {
    u8 pad_0x0[0x40];
    UnkColorEntry** entries_0x40;
    u8 pad_0x48[0xcc - 0x48];
    CVector4D colorsB_0xcc[10];
    CVector4D colorsA_0x16c[10];
};
static_assert(__builtin_offsetof(UnkColorOwner, colorsA_0x16c) == 0x16c, "UnkColorOwner layout");

bool fn_7100193410(UnkEntryListA1_134* list, u32 entryIndex, u32 colorIndex,
                   const base::math::UnkVector4D& colorRef, u32 selector) {
    UnkColorOwner* self = reinterpret_cast<UnkColorOwner*>(list);
    const CVector4D* color = &colorRef;
    UnkColorEntry* entry = self->entries_0x40[entryIndex];
    if (selector == 0) {
        CVector4D* own = &self->colorsA_0x16c[colorIndex];
        fn_710007b518(own, color);
        fn_710007b518(&entry->colorsA_0x240[colorIndex], own);
        if (UnkColorTarget* target = entry->targetsA_0x2e0[colorIndex])
            *target->color_0x30 = *own;
        return true;
    }
    if (selector == 1) {
        CVector4D* own = &self->colorsB_0xcc[colorIndex];
        fn_710007b518(own, color);
        fn_710007b518(&entry->colorsB_0x14c[colorIndex], own);
        if (UnkColorTarget* target = entry->targetsB_0x1f0[colorIndex])
            *target->color_0x30 = *own;
        return true;
    }
    return false;
}
