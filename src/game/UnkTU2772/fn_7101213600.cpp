// UnkTU2772 (nn::atk): finds the first 12-byte entry (count at +0x10, entries from +0x14) with id 0x7000 and
// returns the object its offset names, or null for offset 0. Names and layouts are placeholders.
#include "game/UnkAtkEntry.h"
#include "game/UnkAtkTable.h"
#include "types.h"

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void* fn_7101213600(UnkAtkTable* self) {
    UnkAtkEntry* it = self->entries;
    for (u32 remaining = self->count; remaining != 0; --remaining, ++it) {
        if (it->id == 0x7000)
            return it->offset != 0 ? reinterpret_cast<u8*>(self) + it->offset : nullptr;
    }
    return nullptr;
}
