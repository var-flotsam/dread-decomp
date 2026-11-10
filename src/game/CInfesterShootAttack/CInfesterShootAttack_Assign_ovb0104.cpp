// CInfesterShootAttack's unit (unit 887): a record's copy assignment (plain fields, then the weak
// reference at +0x50 unless assigned to itself, then the word at +0x58). Names and layouts are
// placeholders.
#include "game_s2/UnkWeakHandle.h"

struct UnkInfesterShootRecord_b0104 {
    u8 pad_0x0[0x8];
    u8 field_0x8[32];
    u64 field_0x28;
    u32 field_0x30;
    u8 pad_0x34[0xc];
    u8 field_0x40[16];
    UnkWeakHandle* field_0x50;
    u64 field_0x58;
};

UnkInfesterShootRecord_b0104* fn_71005c7640(UnkInfesterShootRecord_b0104* self, const UnkInfesterShootRecord_b0104* other) {
    __builtin_memcpy(self->field_0x8, other->field_0x8, 32);
    u64 field_0x28 = other->field_0x28;
    u32 field_0x30 = other->field_0x30;
    self->field_0x30 = field_0x30;
    self->field_0x28 = field_0x28;
    __builtin_memcpy(self->field_0x40, other->field_0x40, 16);
    if (self != other)
        CopyWeakRef(&self->field_0x50, other->field_0x50);
    self->field_0x58 = other->field_0x58;
    return self;
}
