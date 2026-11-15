// Unit 83 (base::reflection): returns the string-id slot at `index` of a growable list, growing it first
// (rounded up to its step) and filling new slots with the empty string id. Names and layouts are
// placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"

base::UnkStrIdMember* fn_71000ce740(base::global::CRntVector<base::UnkStrIdMember>* list, u32 index) {
    // The grower is declared on the same layout under another name.
    base::UnkStrIdVec* vec = reinterpret_cast<base::UnkStrIdVec*>(list);
    if (list->field_0x14 && list->count <= index) {
        u32 capacity = list->capacity;
        if (capacity <= index && list->step != 0)
            capacity = (index / list->step + 1) * list->step;
        fn_710000542c(vec, capacity);
        u32 want = index + 1;
        fn_710000542c(vec, want);
        for (u32 i = list->count; i < want; ++i) {
            base::UnkStrIdMember* slot = &list->data[i];
            slot->data = nullptr;
            if (!g_unk_7101d47108)
                base::global::UnkEnsureStringPool();
            slot->data = &g_unk_7101d47108->empty;
        }
        list->count = want;
    }
    return &list->data[index];
}
