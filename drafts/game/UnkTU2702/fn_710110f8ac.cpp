// Unit 2702 (CAIComponent): drops an object from the AI's list of tracked objects (the list at +0x14520), if
// it is in it, then forwards the AI's link at +0x30 to the list at +0x1928. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CInputComponent.h"
#include "types.h"

struct UnkAiTracked_l10f8ac {
    u8 pad_0x0[0x30];
    UnkWeakTarget* field_0x30;
    u8 pad_0x38[0x1928 - 0x38];
    u8 field_0x1928;
    u8 pad_0x1929[0x14520 - 0x1929];
    base::global::CRntVector<void*> tracked;
};
static_assert(__builtin_offsetof(UnkAiTracked_l10f8ac, tracked) == 0x14520, "UnkAiTracked_l10f8ac layout");

// The first entry equal to `object`, or the end of the list when it is not in it.
static void** Find_l10f8ac(base::global::CRntVector<void*>& list, void* object) {
    for (u64 i = 0; i != list.count; ++i) {
        if (list.data[i] == object)
            return &list.data[i];
    }
    return list.data + list.count;
}

// Removes the first entry equal to `object`, moving the later ones down.
static void Remove_l10f8ac(base::global::CRntVector<void*>& list, void* object) {
    u32 n = list.count;
    for (u32 i = 0; i < n; ++i) {
        if (list.data[i] == object) {
            void** p = &list.data[i];
            __builtin_memcpy(p, p + 1, (u64)(n - i - 1) * sizeof(void*));
            --list.count;
            return;
        }
    }
}

void fn_710110f8ac(void* self, void* object) {
    UnkAiTracked_l10f8ac* ai = static_cast<UnkAiTracked_l10f8ac*>(self);
    base::global::CRntVector<void*>& list = ai->tracked;
    if (Find_l10f8ac(list, object) == list.data + list.count)
        return;
    Remove_l10f8ac(list, object);
    fn_7100874460(&ai->field_0x1928, &ai->field_0x30);
}
