// From unit 2613 of tools/tu_map.py (0x7100fb2988-0x7100fb7880, base::reflection names): the entry of
// an object's array at +0x10 whose id (+0x10 of the entry) is `*id`, or null. The array is read
// through its out-of-line accessor fn_7100fa916c (base::UnkVector's GrowAt). Names are placeholders.
#include "base/UnkVector.h"

struct UnkIdEntry_7100fb534c {
    u8 pad_0x0[0x10];
    u64 id;
};

struct UnkIdEntries_7100fb534c {
    u8 pad_0x0[0x10];
    base::UnkVector<UnkIdEntry_7100fb534c*> entries;  // 0x10
};

UnkIdEntry_7100fb534c** fn_7100fa916c(base::UnkVector<UnkIdEntry_7100fb534c*>* array, u32 index);

UnkIdEntry_7100fb534c* fn_7100fb534c(UnkIdEntries_7100fb534c* self, const u64* id) {
    for (u32 i = 0; i < self->entries.count; ++i) {
        UnkIdEntry_7100fb534c** slot = fn_7100fa916c(&self->entries, i);
        if ((*slot)->id == *id)
            return *slot;
    }
    return nullptr;
}
