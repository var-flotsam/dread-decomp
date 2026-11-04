// CCooldownXBossAIComponent's unit: a step that runs the base node routine fn_71003ace0c, passes the object
// at +0x10 of every entry's (0x40 bytes, +0x30) target in the list at +0x15658 to fn_7100232c68, calls
// fn_7100ed78c0 on the sound player g_unk_71726af0b0 and passes the component to fn_7101199818 (as
// CChozoCommanderAIComponent_Step_A2_b69.cpp). Names and layouts are placeholders.
#include "types.h"

struct UnkNode;

void fn_71003ace0c(UnkNode*);
void fn_7100ed78c0(void* player);
void fn_7100232c68(void*);
void fn_7101199818(void* manager, void* component);
extern void* g_unk_71726af0b0;
extern void* g_unk_7101d47330;

struct UnkCooldownTarget_A2_b70 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkCooldownEntry_A2_b70 {
    u8 pad_0x0[0x30];
    UnkCooldownTarget_A2_b70* target;
    u8 pad_0x38[0x8];
};

struct UnkCooldownList_A2_b70 {
    UnkCooldownEntry_A2_b70* data;
    u8 pad_0x8[0x8];
    u32 count;
};

struct UnkCooldownXBossAI_A2_b70 {
    u8 pad_0x0[0x15658];
    UnkCooldownList_A2_b70 list;  // 0x15658
};

void fn_71004d14ac(UnkCooldownXBossAI_A2_b70* self) {
    fn_71003ace0c(reinterpret_cast<UnkNode*>(self));
    UnkCooldownEntry_A2_b70* end = self->list.data + self->list.count;
    for (UnkCooldownEntry_A2_b70* it = self->list.data; it != end; ++it) {
        if (it->target && it->target->field_0x10)
            fn_7100232c68(it->target->field_0x10);
    }
    fn_7100ed78c0(g_unk_71726af0b0);
    fn_7101199818(g_unk_7101d47330, self);
}
