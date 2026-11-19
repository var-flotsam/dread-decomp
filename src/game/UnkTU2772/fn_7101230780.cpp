// UnkTU2772 (nn::atk): returns the object an indexed reference in the table at +0x14 names, if its type is 0x4902
// and the index is in range. Names and layouts are placeholders.
#include "types.h"

struct UnkAtkRef {
    u16 type;  // 0x0
    u16 pad_0x2;
    s32 offset;  // 0x4
};
struct UnkAtkRefTable {
    u32 count;  // 0x0
    UnkAtkRef refs[1];  // 0x4
};
struct UnkAtkRefOwner_l230780 {
    u8 pad_0x0[0x14];
    s32 tableOffset;  // 0x14
};

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void* fn_7101230780(UnkAtkRefOwner_l230780* self, u32 index) {
    UnkAtkRefTable* table = reinterpret_cast<UnkAtkRefTable*>(reinterpret_cast<u8*>(self) + self->tableOffset);
    if (index >= table->count)
        return nullptr;
    const UnkAtkRef& ref = table->refs[index];
    if (ref.type != 0x4902)
        return nullptr;
    return reinterpret_cast<u8*>(table) + ref.offset;
}
