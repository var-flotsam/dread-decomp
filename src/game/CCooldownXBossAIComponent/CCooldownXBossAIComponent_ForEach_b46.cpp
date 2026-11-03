// CCooldownXBossAIComponent's unit (tools/tu_map.py unit 754): hands every non-null entry of the
// list at +0xc40 (count at +0xc48) of the object at +0x149a0 to fn_710070a75c, with the value at
// +0x30. Names and layouts are placeholders.
#include "types.h"

struct UnkCooldownEntry_b46a1;
struct UnkCooldownArg_b46a1;

struct UnkCooldownList_b46a1 {
    u8 pad_0x0[0xc40];
    UnkCooldownEntry_b46a1** data;
    u32 count;
};

struct UnkCooldownXBossAIComponent_b46a1 {
    u8 pad_0x0[0x30];
    UnkCooldownArg_b46a1* field_0x30;
    u8 pad_0x38[0x149a0 - 0x38];
    UnkCooldownList_b46a1* field_0x149a0;
};

void fn_710070a75c(UnkCooldownEntry_b46a1* entry, UnkCooldownArg_b46a1* arg);

void fn_71004d4a28(UnkCooldownXBossAIComponent_b46a1* self) {
    UnkCooldownList_b46a1* list = self->field_0x149a0;
    for (UnkCooldownEntry_b46a1 **it = list->data, **end = it + list->count; it != end; ++it) {
        if (*it)
            fn_710070a75c(*it, self->field_0x30);
    }
}
