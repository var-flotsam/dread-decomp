// Unit 2702 (CAIComponent): adds a target to the component's list unless it is there, then
// reports it to the list at +0x1928. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CInputComponent.h"
#include "types.h"

struct UnkTarget_l10f97c;

struct UnkAIComponent_l10f97c {
    u8 pad_0x0[0x14548];
    base::global::CRntVector<UnkTarget_l10f97c*> targets;  // 0x14548
};

static inline u64 IndexOf(const base::global::CRntVector<UnkTarget_l10f97c*>& v, UnkTarget_l10f97c* target) {
    u64 i = 0;
    for (; i < v.count; ++i) {
        if (v.data[i] == target)
            return i;
    }
    return v.count;
}

void fn_710110f97c(UnkAIComponent_l10f97c* self, UnkTarget_l10f97c* target) {
    base::global::CRntVector<UnkTarget_l10f97c*>& targets = self->targets;
    if (IndexOf(targets, target) != targets.count)
        return;
    targets.PushBack(target);
    fn_7100874460(reinterpret_cast<char*>(self) + 0x1928,
                  reinterpret_cast<UnkWeakTarget**>(reinterpret_cast<char*>(self) + 0x30));
}
