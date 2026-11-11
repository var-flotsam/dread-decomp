// Unit 1125: the constructor of an object built on the unit 1124 object (fn_7100703814, which takes the id),
// with an embedded kind-8 block (its own vtable) at +0xb0. Names and layouts are placeholders.
#include "game/UnkTU1124.h"
#include "types.h"

struct UnkUnit1124Object;

extern void* g_unk_7101a6be90[];  // the object's vtable
extern void* g_unk_7101a6bf40[];  // the embedded block's vtable

struct UnkBlock_b0187f {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    u32 pad_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20[2];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u32 pad_0x38;
    u8 field_0x3c;
    u8 pad_0x3d[3];
    u8 field_0x40[13];
    u8 pad_0x4d[3];
    u64 field_0x50;
    u32 field_0x58;
};

struct UnkUnit1125Object_b0187f {
    void** vtable;
    u8 base_0x8[0xa8];
    UnkBlock_b0187f block;  // 0xb0
};

void fn_7100703cf4(UnkUnit1125Object_b0187f* self, u32 id) {
    fn_7100703814(reinterpret_cast<UnkUnit1124Object*>(self), id);
    self->vtable = &g_unk_7101a6be90[2];
    UnkBlock_b0187f& b = self->block;
    b.field_0x1c = 0;
    __builtin_memset(b.field_0x8, 0, sizeof(b.field_0x8));
    b.field_0x20[0] = 0;
    b.field_0x20[1] = 0;
    b.field_0x30 = 0;
    b.vtable = &g_unk_7101a6bf40[2];
    b.field_0x3c = 0;
    __builtin_memset(b.field_0x40, 0, sizeof(b.field_0x40));
    b.field_0x50 = 0;
    b.field_0x34 = 8;
    b.field_0x58 = 0;
}
