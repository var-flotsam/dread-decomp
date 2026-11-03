// The Shakernaut AI component's constructor (tools/tu_map.py: the CShakernautAIComponent unit): base
// constructor, vtable, two members constructed out of line, then its state block at +0x14800 (two
// timers at -1, two allocator-backed lists, a cleared range and an id of -1), then a virtual call.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101a62f60[];

struct UnkShakernautList_b46a2 {
    u32 capacity;
    u8 flag;
    u32 grow;
    base::UnkAllocator* allocator;
};

struct UnkShakernautAI_b46a2 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    u8 pad_0x8[0x146d0 - 0x8];
    u8 field_0x146d0[0x68];
    u8 field_0x14738[0xc8];
    u32 field_0x14800;
    u8 pad_0x14804[4];
    f32 field_0x14808;
    f32 field_0x1480c;
    u8 field_0x14810[0x10];
    u64 field_0x14820;
    UnkShakernautList_b46a2 list_0x14828;
    u8 field_0x14840[0x10];
    UnkShakernautList_b46a2 list_0x14850;
    u8 pad_0x14868[0x14];
    u8 field_0x1487c[0x10];
    u8 pad_0x1488c[4];
    u64 field_0x14890;
    s64 field_0x14898;
};

void fn_71006618d8(UnkShakernautAI_b46a2* self);
void fn_71006a41b8(void* member);
void fn_71006a6de4(void* member);

void fn_71006b21a0(UnkShakernautAI_b46a2* self) {
    fn_71006618d8(self);
    *reinterpret_cast<void**>(self) = g_unk_7101a62f60 + 0x10;
    fn_71006a41b8(self->field_0x146d0);
    fn_71006a6de4(self->field_0x14738);
    self->field_0x14800 = 1;
    self->field_0x14808 = -1.0f;
    self->field_0x1480c = -1.0f;
    self->field_0x14820 = 0;
    __builtin_memset(self->field_0x14810, 0, sizeof(self->field_0x14810));
    self->list_0x14828.capacity = 0xa;
    self->list_0x14828.flag = 1;
    self->list_0x14828.grow = 0xf;
    self->list_0x14828.allocator = fn_7100083990();
    __builtin_memset(self->field_0x14840, 0, sizeof(self->field_0x14840));
    self->list_0x14850.capacity = 0xa;
    self->list_0x14850.flag = 1;
    self->list_0x14850.grow = 0xf;
    self->list_0x14850.allocator = fn_7100083990();
    __builtin_memset(self->field_0x1487c, 0, sizeof(self->field_0x1487c));
    self->field_0x14890 = 0;
    self->field_0x14898 = -1;
    self->vf_0x18();
}
