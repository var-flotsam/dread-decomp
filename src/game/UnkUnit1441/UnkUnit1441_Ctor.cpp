// Unit 1441 of tools/tu_map.py (0x710096939c-0x7100969ff4, no name hint): the constructor over the base
// of unit 1440 (fn_7100969000): the vtable, the member at +0x90 (its vtable, cleared flags and runs, mode
// 2), the listener at +0xd0 (vtable unk_7101a22d48) and cleared fields after it. Names and layouts are
// placeholders.
#include "types.h"

void fn_7100969000(char* self, u32 arg);
extern void* g_unk_7101a85148[];  // the class's vtable
extern void* g_unk_7101a2e1c0[];  // the member's vtable
extern void* g_unk_7101a22d48[];  // the embedded listener's vtable

struct UnkUnit1441 {
    void** vtable;
    u8 pad_0x8[0x88];
    void** field_0x90;
    u8 pad_0x98[4];
    bool field_0x9c;
    u8 pad_0x9d[7];
    bool field_0xa4;
    u8 pad_0xa5[3];
    u8 field_0xa8[0xd];
    u8 pad_0xb5[3];
    u8 field_0xb8[0xd];
    u8 pad_0xc5[3];
    u32 field_0xc8;
    u8 pad_0xcc[4];
    void** field_0xd0;
    u64 field_0xd8;
    u64 field_0xe0;
    u64 field_0xe8;
    u64 field_0xf0;
    u32 field_0xf8;
    u8 pad_0xfc[0x10c - 0xfc];
    u8 field_0x10c[0xc];
};
static_assert(__builtin_offsetof(UnkUnit1441, field_0x10c) == 0x10c, "UnkUnit1441 layout");

void fn_710096939c(UnkUnit1441* self, u32 arg) {
    fn_7100969000(reinterpret_cast<char*>(self), arg);
    self->vtable = g_unk_7101a85148 + 2;
    self->field_0x9c = false;
    self->field_0xa4 = false;
    __builtin_memset(self->field_0xa8, 0, sizeof(self->field_0xa8));
    __builtin_memset(self->field_0xb8, 0, sizeof(self->field_0xb8));
    self->field_0x90 = g_unk_7101a2e1c0 + 2;
    self->field_0xc8 = 2;
    self->field_0xd0 = g_unk_7101a22d48 + 2;
    self->field_0xd8 = 0;
    __builtin_memset(self->field_0x10c, 0, sizeof(self->field_0x10c));
    self->field_0xe0 = 0;
    self->field_0xe8 = 0;
    self->field_0xf0 = 0;
    self->field_0xf8 = 0;
}
