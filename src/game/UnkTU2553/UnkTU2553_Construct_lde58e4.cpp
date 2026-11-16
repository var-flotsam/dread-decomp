// Unit 2553 (CGameManager::CTunableGameManager): the default constructor of a table of 13 slots, each
// three empty arrays (growth step 10, allocator flags 0xf) and a cleared flag. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "types.h"

struct UnkSlotEntry_lde58e4;

struct UnkSlot_lde58e4 {
    base::global::CRntVector<UnkSlotEntry_lde58e4*> field_0x0{0xf};
    base::global::CRntVector<UnkSlotEntry_lde58e4*> field_0x28{0xf};
    base::global::CRntVector<UnkSlotEntry_lde58e4*> field_0x50{0xf};
    bool field_0x78 = false;
};
static_assert(sizeof(UnkSlot_lde58e4) == 0x80, "UnkSlot_lde58e4 layout");

struct UnkSlotTable_lde58e4 {
    UnkSlot_lde58e4 slots[13];
};

void fn_7100de58e4(UnkSlotTable_lde58e4* self) {
    for (UnkSlot_lde58e4& slot : self->slots)
        new (&slot) UnkSlot_lde58e4;
}
