// CAIManager's unit: whether any entry of the inline list at +0x8 (a u32 count followed directly by
// the pointers, at +0xc) leads to an object (entry +0x10, then +0x30) whose flag at +0x1b4 is set and
// that either has its +0x1b0 flag clear or a pointer at +0x1a8. Names and layouts are placeholders.
#include "types.h"

struct UnkAIManagerTarget_A5_b69 {
    u8 pad_0x0[0x1a8];
    void* field_0x1a8;
    bool field_0x1b0;
    u8 pad_0x1b1[3];
    bool field_0x1b4;
};
struct UnkAIManagerLink_A5_b69 {
    u8 pad_0x0[0x30];
    UnkAIManagerTarget_A5_b69* target;
};
struct UnkAIManagerEntry_A5_b69 {
    u8 pad_0x0[0x10];
    UnkAIManagerLink_A5_b69* link;
};

#pragma pack(push, 4)
struct UnkAIManagerList_A5_b69 {
    u8 pad_0x0[0x8];
    u32 count;
    UnkAIManagerEntry_A5_b69* items[1];  // 0xc, 4-byte aligned
};
#pragma pack(pop)

bool fn_7100dae840(UnkAIManagerList_A5_b69* self) {
    u64 bytes = (u64)self->count * sizeof(void*);
    asm("" : "+r"(bytes));
    for (UnkAIManagerEntry_A5_b69 **it = self->items, **end = (UnkAIManagerEntry_A5_b69**)((char*)it + bytes); it != end;
         ++it) {
        UnkAIManagerTarget_A5_b69* target = (*it)->link->target;
        if (target && target->field_0x1b4 && (!target->field_0x1b0 || target->field_0x1a8))
            return true;
    }
    return false;
}
