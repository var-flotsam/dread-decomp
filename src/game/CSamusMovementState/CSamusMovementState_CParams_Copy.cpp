// CSamusMovementState::CParams: the second copy of its copy constructor (fn_71009b2e58 in
// CSamusMovementState_CParams.cpp), reached through the reflection descriptor's slot 0x38. Stores the
// vtable, then copies the members field by field. Names and layouts are placeholders.
#include "game/CAbilityGhostAura.h"
#include "types.h"

struct UnkSamusMovementParamsCopy {
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

void fn_71009b2ebc(void* dst, const void* src) {
    UnkSamusMovementParamsCopy* self = static_cast<UnkSamusMovementParamsCopy*>(dst);
    const UnkSamusMovementParamsCopy* other = static_cast<const UnkSamusMovementParamsCopy*>(src);
    self->vtable = g_unk_7101a22cb8 + 2;
    self->field_0x8 = other->field_0x8;
    __builtin_memcpy(&self->field_0x10, &other->field_0x10, 0x1c);
    self->field_0x2c = other->field_0x2c;
    self->field_0x30 = other->field_0x30;
    self->field_0x38 = other->field_0x38;
    self->field_0x3c = other->field_0x3c;
    self->field_0x40 = other->field_0x40;
    self->field_0x44 = other->field_0x44;
}
