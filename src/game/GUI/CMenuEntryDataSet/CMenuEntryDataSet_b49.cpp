// GUI::CMenuEntryDataSet (tools/tu_map.py unit 2760): the index of the first entry of the list at +0x10
// whose name (the first word of the entry) equals `name` (fn_710007f120), or -1. The element accessor
// is the shared out-of-line copy (src/clones/, fn_7100affc6c). Names and layout are placeholders.
#include "UnkClone.h"
#include "base/UnkVector.h"
#include "types.h"

// As src/base/reflection/UnkReflArrayEqual.cpp declares it.
struct UnkCompared_0x30;
bool fn_710007f120(const UnkCompared_0x30*, const UnkCompared_0x30*);

struct UnkClone_fn_7100affc6c;
using UnkMenuEntrySlot_b49a6 = UnkCloneElem<UnkClone_fn_7100affc6c, 8>;
extern template UnkMenuEntrySlot_b49a6& base::UnkVector<UnkMenuEntrySlot_b49a6>::GrowAt(u32);

struct UnkMenuEntryDataSet_b49a6 {
    u8 pad_0x0[0x10];
    base::UnkVector<UnkMenuEntrySlot_b49a6> entries;  // 0x10
};

s32 fn_710120201c(UnkMenuEntryDataSet_b49a6* self, const UnkCompared_0x30* name) {
    s32 found = -1;
    u32 i = 0;
    while (found < 0 && i < self->entries.count) {
        const UnkCompared_0x30* entry = *reinterpret_cast<const UnkCompared_0x30**>(self->entries.GrowAt(i).data);
        found = fn_710007f120(entry, name) ? (s32)i : found;
        ++i;
    }
    return found;
}
