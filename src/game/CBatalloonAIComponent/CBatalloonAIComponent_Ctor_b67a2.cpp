// CBatalloonAIComponent unit (tools/tu_map.py, the name from reflection): the in-place constructor its
// reflection descriptor points to (fn_0x28). After the base AI component constructor (fn_71005a2480) it sets
// the vtable and clears the fields from +0x146d8 to +0x14708 (two of them set to -1), then runs the class entries
// (fn_7100023854). Defined under the `void()` symbol src/reflection/CBatalloonAIComponent.cpp declares (the b64a4
// alias pattern). Names and layouts are placeholders.
#include "types.h"

struct UnkNode;
void fn_7100023854(UnkNode* node);

struct UnkBatalloonAI_b67a2;
void fn_71005a2480(UnkBatalloonAI_b67a2* self);
extern void* g_unk_7101a2f188[];

struct UnkBatalloonBlock_b67a2 {
    u16 field_0x0;
    f32 field_0x4;
    u8 field_0x8;
};

struct UnkBatalloonAI_b67a2 {
    void* vtable;
    u8 pad_0x8[0x146d8 - 0x8];
    UnkBatalloonBlock_b67a2 block;
    u32 range[8];
    f32 field_0x14704;
};

void fn_71003df25c_b67a2(UnkBatalloonAI_b67a2* self) asm("_Z13fn_71003df25cv");
void fn_71003df25c_b67a2(UnkBatalloonAI_b67a2* self) {
    fn_71005a2480(self);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a2f188) + 0x10;
    self->block.field_0x0 = 0;
    self->block.field_0x8 = 0;
    self->block.field_0x4 = -1.0f;
    __builtin_memset(self->range, 0, sizeof(self->range));
    self->field_0x14704 = -1.0f;
    fn_7100023854(reinterpret_cast<UnkNode*>(self));
}
