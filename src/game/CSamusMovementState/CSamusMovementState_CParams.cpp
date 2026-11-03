// CSamusMovementState::CParams: its copy constructor (fn_71009b2e58). Stores the vtable, then copies the
// members field by field, +0x10 to +0x2c as one block; the word at +0x34 is padding. Names and
// layouts are placeholders.
#include "types.h"

extern void* g_unk_7101a22cb8[];

struct UnkSamusMovementParams {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    u32 field_0x30;
    u32 pad_0x34;
    u32 field_0x38;
    u32 field_0x3c;
    u32 field_0x40;
    u32 field_0x44;
};

UnkSamusMovementParams* fn_71009b2e58(UnkSamusMovementParams* self, const UnkSamusMovementParams* other) {
    self->vtable = g_unk_7101a22cb8 + 2;
    self->field_0x8 = other->field_0x8;
    __builtin_memcpy(&self->field_0x10, &other->field_0x10, 0x1c);
    self->field_0x2c = other->field_0x2c;
    self->field_0x30 = other->field_0x30;
    self->field_0x38 = other->field_0x38;
    self->field_0x3c = other->field_0x3c;
    self->field_0x40 = other->field_0x40;
    self->field_0x44 = other->field_0x44;
    return self;
}
