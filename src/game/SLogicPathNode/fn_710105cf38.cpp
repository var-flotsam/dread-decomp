// SLogicPathNode unit: the 0x50-byte element's move constructor out of line (UnkItem,
// include/game_s2/UnkItem50.h). Names and layouts are placeholders.
#include "game_s2/UnkItem50.h"

void fn_710105cf38(UnkItem* self, UnkItem* other) {
    self->vtable = g_unk_7101cac938 + 2;
    self->field_0x8.field_0x0 = other->field_0x8.field_0x0;
    self->field_0x8.field_0x8 = other->field_0x8.field_0x8;
    self->field_0x8.field_0xc = other->field_0x8.field_0xc;
    self->field_0x8.field_0x10 = other->field_0x8.field_0x10;
    self->field_0x8.field_0x14 = other->field_0x8.field_0x14;
    self->field_0x8.field_0x18 = other->field_0x8.field_0x18;
    self->field_0x8.field_0x20 = other->field_0x8.field_0x20;
    u8 flag = other->field_0x30;
    other->field_0x8.field_0x0 = 0;
    flag = other->field_0x30;
    other->field_0x8.field_0x8 = 0;
    other->field_0x8.field_0xc = 0;
    self->field_0x30 = flag;
    self->field_0x34 = 0;
    self->field_0x3c = 0;
    self->field_0x44 = 0;
}
