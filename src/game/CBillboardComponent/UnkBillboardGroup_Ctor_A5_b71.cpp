// Unit 1161 (no reflection hint; its only caller, fn_7100728df0, is in the CBillboardComponent unit): the
// constructor of an object with a vtable, cleared fields, the default allocator at +0xd8, a member built by
// fn_71007457cc at +0xe8 and a back pointer plus callback (fn_7100745fd8) at +0x138. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a6da30[];
void fn_71007457cc(void* self);
void fn_7100745fd8();

struct UnkBillboardGroup_A5_b71 {
    void* vtable;
    u64 field_0x8;
    u8 pad_0x10[0x10];
    s32 field_0x20;
    u8 field_0x24;
    u8 pad_0x25[0x7];
    f32 field_0x2c;
    u8 field_0x30[13];
    u8 pad_0x3d[0x3];
    u8 field_0x40[0x24];
    u8 pad_0x64[0x4];
    u8 field_0x68;
    u8 pad_0x69[0x4];
    u8 field_0x6d;
    u8 field_0x6e;
    u8 pad_0x6f[0x1];
    u8 field_0x70[0x3c];
    u8 pad_0xac[0xc];
    u8 field_0xb8[0x10];
    u32 field_0xc8;
    u8 field_0xcc;
    u8 pad_0xcd[0x3];
    u32 field_0xd0;
    u8 pad_0xd4[0x4];
    base::UnkAllocator* allocator;
    u8 pad_0xe0[0x8];
    u8 member[0x50];
    void* owner;
    void (*callback)();
};
static_assert(__builtin_offsetof(UnkBillboardGroup_A5_b71, owner) == 0x138, "layout");

void fn_7100745a7c(UnkBillboardGroup_A5_b71* self) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a6da30) + 0x10;
    self->field_0x8 = 0;
    self->field_0x20 = -1;
    self->field_0x24 = 0;
    self->field_0x2c = 2.0f;
    self->field_0x68 = 0;
    self->field_0x6d = 0;
    self->field_0x6e = 0;
    __builtin_memset(self->field_0xb8, 0, sizeof(self->field_0xb8));
    __builtin_memset(self->field_0x30, 0, sizeof(self->field_0x30));
    __builtin_memset(self->field_0x40, 0, sizeof(self->field_0x40));
    __builtin_memset(self->field_0x70, 0, sizeof(self->field_0x70));
    self->field_0xc8 = 10;
    self->field_0xcc = 1;
    self->field_0xd0 = 15;
    self->allocator = fn_7100083990();
    fn_71007457cc(self->member);
    self->owner = self;
    self->callback = fn_7100745fd8;
}
