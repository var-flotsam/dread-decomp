// Two identical assignment operators in the CDamageSourceFactor unit: unless the source is the object
// itself, everything after the vtable (0x8-0x6c) is copied. Names and layouts are placeholders.
#include "types.h"

struct UnkDamageSourceFactor {
    void** vtable;
    u64 field_0x8;
    u8 field_0x10[0x20];
    u8 field_0x30[0x20];
    u8 field_0x50[0x1c];
};

static inline __attribute__((always_inline)) UnkDamageSourceFactor* AssignFactor(
    UnkDamageSourceFactor* self, const UnkDamageSourceFactor* other) {
    if (self != other) {
        self->field_0x8 = other->field_0x8;
        __builtin_memcpy(self->field_0x10, other->field_0x10, sizeof(self->field_0x10));
        __builtin_memcpy(self->field_0x30, other->field_0x30, sizeof(self->field_0x30));
        __builtin_memcpy(self->field_0x50, other->field_0x50, sizeof(self->field_0x50));
    }
    return self;
}

// Declared as the reflection traits (src/reflection/CDamageSourceFactor.cpp) declare them.
void* fn_71008c307c(void* dst, const void* src) {
    return AssignFactor(static_cast<UnkDamageSourceFactor*>(dst), static_cast<const UnkDamageSourceFactor*>(src));
}
void* fn_71008c30c8(void* dst, const void* src) {
    return AssignFactor(static_cast<UnkDamageSourceFactor*>(dst), static_cast<const UnkDamageSourceFactor*>(src));
}
