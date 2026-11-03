// CCutsceneTriggerComponent constructor (tools/tu_map.py, the name from reflection; the same shape as
// fn_7100a1482c in CAutsharpSpawnPointComponent.cpp): base constructor, vtable, the container at +0xac0
// (base vtable, empty array with step 10 and flags 0xf, final vtable, allocator), two flags, then the
// post-construction virtual call (slot 3). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_7100a46a68(char*);
extern void* g_unk_7101a20998[];
extern void* g_unk_7101a20950[];
extern void* g_unk_7101a8df00[];

struct UnkCutsceneTriggerComponent {
    void** vtable;
    u8 pad_0x8[0xac0 - 8];
    void** field_0xac0;
    u64 field_0xac8;
    u64 field_0xad0;
    u32 field_0xad8;
    bool field_0xadc;
    u32 field_0xae0;
    base::UnkAllocator* field_0xae8;
    u32 field_0xaf0;
    u64 field_0xaf8;
    bool field_0xb00;
    bool field_0xb01;
};

void fn_7100a2f3c8(UnkCutsceneTriggerComponent* self) {
    fn_7100a46a68(reinterpret_cast<char*>(self));
    self->vtable = g_unk_7101a8df00 + 2;
    self->field_0xac0 = g_unk_7101a20998 + 2;
    self->field_0xac8 = 0;
    self->field_0xad0 = 0;
    self->field_0xad8 = 10;
    self->field_0xadc = true;
    self->field_0xae0 = 0xf;
    self->field_0xae8 = fn_7100083990();
    self->field_0xaf0 = 0;
    self->field_0xac0 = g_unk_7101a20950 + 2;
    self->field_0xaf8 = 0;
    self->field_0xb00 = true;
    self->field_0xb01 = false;
    reinterpret_cast<void (*)(UnkCutsceneTriggerComponent*)>(self->vtable[3])(self);
}
