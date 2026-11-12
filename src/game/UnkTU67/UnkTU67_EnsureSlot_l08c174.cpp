// UnkTU67: returns the path-id slot at `index` of a growable list, growing it first (rounded up to
// its step) and filling new slots with the empty file-path id. Names and layouts are placeholders.

#include "base/UnkDestructors.h"
#include "base/UnkResPathVec.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"

using UnkPathIdMember = base::UnkStrIdMemberOf<g_unk_7101d47200>;  // a file-path id

// The same holder as UnkEnsureFilePathPool, as the original's own copy of that static.
inline void UnkEnsureFilePathPool_l08c174() {
    static base::global::UnkStringPoolHolder s_holder asm("g_unk_7101d47208")(
        fn_7100080570(), &g_unk_7101d47200);
    fn_710007fdb4();
}

UnkPathIdMember* fn_710008c174(base::UnkVector<UnkPathIdMember>* list, u32 index) {
    // The grower is declared on the same layout under another name.
    UnkResPathVec* vec = reinterpret_cast<UnkResPathVec*>(list);
    if (list->field_0x14 && list->count <= index) {
        u32 capacity = list->capacity;
        if (capacity <= index && list->step != 0)
            capacity = (index / list->step + 1) * list->step;
        fn_7100018450(vec, capacity);
        u32 want = index + 1;
        fn_7100018450(vec, want);
        for (u32 i = list->count; i < want; ++i) {
            UnkPathIdMember* slot = &list->data[i];
            slot->data = nullptr;
            if (!g_unk_7101d47200)
                UnkEnsureFilePathPool_l08c174();
            slot->data = &g_unk_7101d47200->empty;
        }
        list->count = want;
    }
    return &list->data[index];
}
