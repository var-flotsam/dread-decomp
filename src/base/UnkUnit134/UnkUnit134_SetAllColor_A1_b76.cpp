// Unit 134 of tools/tu_map.py: applies a colour (converted to a vector by fn_7100030878) to every
// entry of the list reached through +0x160 then +0x78 (count at +0x24) through fn_7100193410, stopping
// the calls after the first failure, and answers whether all succeeded (fn_7100146c50 and its
// identical copy fn_71001471b4). Names and layouts are placeholders.
#include "base/color/CColor4F.h"
#include "game/UnkTU211.h"
#include "types.h"

struct UnkEntryListA1_134 {
    u8 pad_0x0[0x24];
    u32 count;  // 0x24
};

struct UnkHolderA1_134 {
    u8 pad_0x0[0x78];
    UnkEntryListA1_134* list;  // 0x78
};

struct UnkOwnerA1_134 {
    u8 pad_0x0[0x160];
    UnkHolderA1_134* holder;  // 0x160
};

static inline bool SetAllA1(UnkOwnerA1_134* self, u32 slot, const base::color::CColor4F* color, u32 flag) {
    u32 count = self->holder->list->count;
    bool ok = true;
    for (u32 i = 0; i < count; ++i) {
        if (ok) {
            ok = fn_7100193410(self->holder->list, i, slot, fn_7100030878(color), flag);
        } else {
            ok = false;
        }
    }
    return ok;
}

bool fn_7100146c50(UnkOwnerA1_134* self, u32 slot, const base::color::CColor4F* color, u32 flag) {
    return SetAllA1(self, slot, color, flag);
}

bool fn_71001471b4(UnkOwnerA1_134* self, u32 slot, const base::color::CColor4F* color, u32 flag) {
    return SetAllA1(self, slot, color, flag);
}
