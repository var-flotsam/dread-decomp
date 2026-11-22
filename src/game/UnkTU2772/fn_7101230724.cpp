// UnkTU2772 (nn::atk): returns the object an indexed reference in the table at +0xc names, if its type is 0x4900
// and the index is in range. Names and layouts are placeholders.
#include "game/UnkAtkRef.h"
#include "game/UnkAtkRefTable.h"
#include "types.h"

struct UnkAtkRefOwner_l230724 {
    u8 pad_0x0[0xc];
    s32 tableOffset;  // 0xc
};

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void* fn_7101230724(UnkAtkRefOwner_l230724* self, u32 index) {
    UnkAtkRefTable* table = reinterpret_cast<UnkAtkRefTable*>(reinterpret_cast<u8*>(self) + self->tableOffset);
    if (index >= table->count)
        return nullptr;
    const UnkAtkRef& ref = table->refs[index];
    if (ref.type != 0x4900)
        return nullptr;
    return reinterpret_cast<u8*>(table) + ref.offset;
}
