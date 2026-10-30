// From unit 1124 of tools/tu_map.py (0x710070368c-0x7100703cf4, no reflected class): the constructor
// of a 0xb0-byte object with an id at +0x10 and an embedded object (its own vtable) at +0x70; the rest
// is cleared, with 2 and 1 at +0x28 and +0x2c. Names and layouts are placeholders.
#include "types.h"

extern void* g_unk_7101a6bd88[];  // the object's vtable
extern void* g_unk_7101a24b58[];  // the embedded object's vtable

struct UnkUnit1124Object {
    void** vtable;
    u64 field_0x8;
    u32 id;  // 0x10
    u8 pad_0x14[4];
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u64 field_0x48;
    u32 field_0x50;
    u8 pad_0x54[4];
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;
    void** inner_vtable;  // 0x70
    u32 field_0x78;
    bool field_0x7c;
    bool field_0x7d[8];  // 0x7d..0x84
    u8 pad_0x85[7];
    bool field_0x8c;
    u8 pad_0x8d[3];
    u64 field_0x90;
    u64 field_0x98;
    bool field_0xa0;
    u8 pad_0xa1[3];
    u32 field_0xa4;
    bool field_0xa8;
};
static_assert(__builtin_offsetof(UnkUnit1124Object, field_0xa8) == 0xa8, "UnkUnit1124Object layout");

// NON_MATCHING(minor): same stores; the original schedules them in another order and keeps the store
// of +0x78 apart from the embedded vtable's.
void fn_7100703814(UnkUnit1124Object* self, u32 id) {
    self->id = id;
    self->field_0x50 = 0;
    self->field_0x18 = 0;
    self->field_0x20 = 0;
    self->field_0x28 = 2;
    self->field_0x2c = 1;
    self->field_0x30 = 0;
    self->field_0x48 = 0;
    self->field_0x38 = 0;
    self->field_0x40 = 0;
    self->vtable = &g_unk_7101a6bd88[2];
    self->field_0x8 = 0;
    self->field_0x8c = false;
    self->field_0xa4 = 0;
    self->field_0x78 = 0;
    self->field_0x7c = false;
    for (bool& b : self->field_0x7d)
        b = false;
    self->field_0xa0 = false;
    self->field_0xa8 = false;
    self->field_0x58 = 0;
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    self->inner_vtable = &g_unk_7101a24b58[2];
    self->field_0x90 = 0;
    self->field_0x98 = 0;
}
