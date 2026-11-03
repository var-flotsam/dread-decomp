// Two identical assignment operators in the CRodomithonXSuckAttack unit: unless the source is the object
// itself, everything after the vtable (0x8-0x3c) is copied. Names and layouts are placeholders.
#include "types.h"

struct UnkRodomithonXSuckParams {
    void** vtable;
    u8 field_0x8[0x20];
    u8 field_0x28[0x14];
};

static inline __attribute__((always_inline)) UnkRodomithonXSuckParams* AssignParams(
    UnkRodomithonXSuckParams* self, const UnkRodomithonXSuckParams* other) {
    if (self != other) {
        __builtin_memcpy(self->field_0x8, other->field_0x8, sizeof(self->field_0x8));
        __builtin_memcpy(self->field_0x28, other->field_0x28, sizeof(self->field_0x28));
    }
    return self;
}

// Declared as the reflection traits (src/reflection/CRodomithonXSuckAttack.cpp) declare them.
void* fn_7100664d7c(void* dst, const void* src) {
    return AssignParams(static_cast<UnkRodomithonXSuckParams*>(dst), static_cast<const UnkRodomithonXSuckParams*>(src));
}
void* fn_7100664da8(void* dst, const void* src) {
    return AssignParams(static_cast<UnkRodomithonXSuckParams*>(dst), static_cast<const UnkRodomithonXSuckParams*>(src));
}
