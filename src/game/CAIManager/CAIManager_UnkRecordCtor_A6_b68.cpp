// CAIManager::CTunableAIManager's unit: constructor of a record with its own vtable (g_unk_7101aa9328): two
// (0, -1) pairs, cleared fields and flags, and an embedded object at +0x68 built by fn_710002f4e4. Names and
// the layout are placeholders.
#include "game/UnkTU39.h"
#include "types.h"

extern void* g_unk_7101aa9328[];

struct UnkAIRecord_A6_b68 {
    void* vtable;
    u32 field_0x8;
    u8 unk_0xc[0x20 - 0xc];
    u64 field_0x20;
    s64 field_0x28;
    u64 field_0x30;
    s64 field_0x38;
    u64 field_0x40;
    u64 field_0x48;
    u64 field_0x50;
    u64 field_0x58;
    bool field_0x60;
    u32 field_0x64;
    u8 field_0x68[0x118 - 0x68];
    u64 field_0x118;
    u32 field_0x120;
    bool field_0x124;
    u64 field_0x128;
    u64 field_0x130;
    u32 field_0x138;
};

void fn_7100dac280(UnkAIRecord_A6_b68* self) {
    self->field_0x20 = 0;
    self->field_0x28 = -1;
    self->field_0x30 = 0;
    self->field_0x38 = -1;
    self->field_0x8 = 0;
    self->field_0x64 = 0;
    self->vtable = reinterpret_cast<char*>(g_unk_7101aa9328) + 0x10;
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    self->field_0x60 = false;
    self->field_0x50 = 0;
    self->field_0x58 = 0;
    fn_710002f4e4(self->field_0x68);
    self->field_0x118 = 0;
    self->field_0x120 = 0;
    self->field_0x124 = false;
    self->field_0x138 = 0;
    self->field_0x128 = 0;
    self->field_0x130 = 0;
}
