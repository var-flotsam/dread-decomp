// Unit 2702 (CAIComponent): adds an object to the AI's list of tracked objects (the list at +0x14520) unless it
// is in it already, then forwards the AI's link at +0x30 to the list at +0x1928. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CInputComponent.h"
#include "types.h"

struct UnkAiTracked_l10f744 {
    u8 pad_0x0[0x30];
    UnkWeakTarget* field_0x30;
    u8 pad_0x38[0x1928 - 0x38];
    u8 field_0x1928;
    u8 pad_0x1929[0x14520 - 0x1929];
    base::global::CRntVector<void*> tracked;

    // Hands the link at +0x30 to the list at +0x1928.
    void Forward() { fn_7100874460(&field_0x1928, &field_0x30); }
};
static_assert(__builtin_offsetof(UnkAiTracked_l10f744, tracked) == 0x14520, "UnkAiTracked_l10f744 layout");

// The first entry equal to `object`, or the end of the list when it is not in it.
static void** Find_l10f744(base::global::CRntVector<void*>& list, void* object) {
    for (u64 i = 0; i != list.count; ++i) {
        if (list.data[i] == object)
            return &list.data[i];
    }
    return list.data + list.count;
}

void fn_710110f744(void* self, void* object) {
    UnkAiTracked_l10f744* ai = static_cast<UnkAiTracked_l10f744*>(self);
    base::global::CRntVector<void*>& list = ai->tracked;
    if (Find_l10f744(list, object) != list.data + list.count)
        return;
    list.PushBack(object);
    ai->Forward();
}
