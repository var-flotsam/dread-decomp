// tools/tu_map.py unit 259 (no class hint): a constructor (fn_71001d81f8) that sets the vtable
// unk_7101a1e7a0, constructs four members and clears the tail. Names and layouts are placeholders.
#include "types.h"

extern void* g_unk_7101a1e7a0[];

void fn_710014e6cc(void* self);
void fn_710014dc2c(void* self);
void fn_710014dd8c(void* self);
void fn_710014f03c(void* self);

struct UnkUnit259Object_b67a1 {
    void* vtable;
    u8 pad_0x8[0x8];
    u8 member_0x10[0xc10];
    u8 member_0xc20[0xc10];
    u8 member_0x1830[0xc10];
    u8 member_0x2440[0xc18];
    u64 field_0x3058;
    u64 field_0x3060;
    f32 field_0x3068;
    f32 field_0x306c;
    u8 pad_0x3070[0xc];
    u32 field_0x307c;
    bool field_0x3080;
    bool field_0x3081;
};
static_assert(__builtin_offsetof(UnkUnit259Object_b67a1, field_0x3081) == 0x3081, "layout");

void fn_71001d81f8(UnkUnit259Object_b67a1* self, u32 value, bool flag) {
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1e7a0) + 0x10;
    fn_710014e6cc(self->member_0x10);
    fn_710014dc2c(self->member_0xc20);
    fn_710014dd8c(self->member_0x1830);
    fn_710014f03c(self->member_0x2440);
    self->field_0x3058 = 0;
    self->field_0x3060 = 0;
    self->field_0x3068 = 0.0f;
    self->field_0x306c = 1.0f;
    self->field_0x3081 = true;
    self->field_0x307c = value;
    self->field_0x3080 = flag;
}
