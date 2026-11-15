// Unit 2702 (CAIComponent): drops an object from the AI's list of tracked objects (the list at +0x14548), if
// it is in it, then forwards the AI's link at +0x30 to the list at +0x1928. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CInputComponent.h"
#include "types.h"

struct UnkAiTracked_l10fae4 {
    u8 pad_0x0[0x30];
    UnkWeakTarget* field_0x30;
    u8 pad_0x38[0x1928 - 0x38];
    u8 field_0x1928;
    u8 pad_0x1929[0x14548 - 0x1929];
    base::global::CRntVector<void*> tracked;
};
static_assert(__builtin_offsetof(UnkAiTracked_l10fae4, tracked) == 0x14548, "UnkAiTracked_l10fae4 layout");

// The index of the first entry equal to `object`, or the count when it is not in the list.
static u64 IndexOf_l10fae4(const base::global::CRntVector<void*>& list, void* object) {
    u64 i = 0;
    for (; i != list.count; ++i) {
        if (list.data[i] == object)
            break;
    }
    return i;
}

void fn_710110fae4(void* self, void* object) {
    UnkAiTracked_l10fae4* ai = static_cast<UnkAiTracked_l10fae4*>(self);
    base::global::CRntVector<void*>& list = ai->tracked;
    if (IndexOf_l10fae4(list, object) == list.count)
        return;
    u64 i = IndexOf_l10fae4(list, object);
    if (i != list.count) {
        __builtin_memcpy(&list.data[(u32)i], &list.data[(u32)i + 1], (u64)(list.count - (u32)i - 1) * sizeof(void*));
        --list.count;
    }
    fn_7100874460(&ai->field_0x1928, &ai->field_0x30);
}
