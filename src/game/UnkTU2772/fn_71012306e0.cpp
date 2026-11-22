// UnkTU2772 (nn::atk): finds the first 12-byte entry (count at +0x10, entries from +0x14) with id 0x6800 and
// returns the object its offset names, or null for offset 0. Names and layouts are placeholders.
#include "game/UnkAtkEntry.h"
#include "game/UnkAtkTable.h"
#include "types.h"

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void* fn_71012306e0(UnkAtkTable* self) {
    UnkAtkEntry* it = self->entries;
    u64 remaining = self->count;
    while (remaining != 0) {
        if (it->id == 0x6800)
            return it->offset != 0 ? reinterpret_cast<u8*>(self) + it->offset : nullptr;
        ++it;
        --remaining;
    }
    return nullptr;
}
