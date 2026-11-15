// Unit 2702: an AI component remembers an object once (a list of the objects it has seen) and registers
// its owner with the item list. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CInputComponent.h"
#include "types.h"

struct UnkRegisterAI_l10f744 {
    u8 pad_0x0[0x30];
    UnkWeakTarget* owner;  // 0x30
    u8 pad_0x38[0x18f0];
    u8 field_0x1928[1];
    u8 pad_0x1929[0x12bf7];
    base::global::CRntVector<void*> seen;  // 0x14520
};
static_assert(__builtin_offsetof(UnkRegisterAI_l10f744, seen) == 0x14520, "layout");

void fn_710110f744(void* component, void* object) {
    UnkRegisterAI_l10f744* self = static_cast<UnkRegisterAI_l10f744*>(component);
    u32 count = self->seen.count;
    void** items = self->seen.data;
    u64 index = 0;
    for (; index != count; ++index)
        if (items[index] == object)
            break;
    if (index != count)
        return;
    self->seen.PushBack(object);
    fn_7100874460(self->field_0x1928, &self->owner);
}
