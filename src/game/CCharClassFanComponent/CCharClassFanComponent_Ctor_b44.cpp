// CCharClassFanComponent (tools/tu_map.py; the name from reflection): the constructor of the
// component. The base constructor fn_7100a46a68, the vtable, cleared state, scales of 1 at +0xad0,
// +0xaf8 and +0xafc, two pooled lists at +0xb08 and +0xb78 (fn_710087dc1c, fn_710087dd34: 10
// nodes per block, flags 0, allocation flags 0x13, the default allocator), then the vtable's slot
// at +0x18 on itself. Names and layout are placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_7100a46a68(char*);
void fn_710087dc1c(void* list, u32 block, u32 flags, u32 alloc_flags, base::UnkAllocator* allocator);
void fn_710087dd34(void* list, u32 block, u32 flags, u32 alloc_flags, base::UnkAllocator* allocator);
extern void* g_unk_7101a7b368[];  // the class's vtable

struct UnkFanComponent_b44a2 {
    void** vtable;
    u8 pad_0x8[0xac8];
    f32 field_0xad0;
    f32 field_0xad4;
    u32 field_0xad8;
    u32 pad_0xadc;
    u64 field_0xae0;
    u64 field_0xae8;
    u64 field_0xaf0;
    f32 field_0xaf8;
    f32 field_0xafc;
    u8 pad_0xb00[0x8];
    u8 field_0xb08[0x70];
    u8 field_0xb78[0x70];
};

void fn_710087f7b0(UnkFanComponent_b44a2* self) {
    fn_7100a46a68(reinterpret_cast<char*>(self));
    self->vtable = g_unk_7101a7b368 + 2;
    self->field_0xad8 = 0;
    self->field_0xae0 = 0;
    self->field_0xae8 = 0;
    self->field_0xaf0 = 0;
    self->field_0xad0 = 1.0f;
    self->field_0xad4 = 0.0f;
    self->field_0xaf8 = 1.0f;
    self->field_0xafc = 1.0f;
    fn_710087dc1c(self->field_0xb08, 10, 0, 0x13, nullptr);
    fn_710087dd34(self->field_0xb78, 10, 0, 0x13, nullptr);
    reinterpret_cast<void (*)(UnkFanComponent_b44a2*)>(self->vtable[3])(self);
}
