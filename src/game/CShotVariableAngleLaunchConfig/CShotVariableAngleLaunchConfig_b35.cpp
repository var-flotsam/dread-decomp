// Two identical copy constructors in the CShotVariableAngleLaunchConfig unit: the base vtable and the
// base's 16 bytes at +8, then the derived vtable and the two 16-byte members at +0x18 and +0x28.
// Names and layouts are placeholders.
#include "types.h"

extern void* g_unk_7101a8ad08[];
extern void* g_unk_7101a8ad38[];

struct UnkPair16 {
    u64 a;
    u64 b;
};
struct UnkVariableAngleLaunchConfig {
    void** vtable;
    UnkPair16 base_0x8;
    UnkPair16 field_0x18;
    UnkPair16 field_0x28;
};

static inline __attribute__((always_inline)) UnkVariableAngleLaunchConfig* CopyConfig(
    UnkVariableAngleLaunchConfig* self, const UnkVariableAngleLaunchConfig* other) {
    self->vtable = &g_unk_7101a8ad08[2];
    __builtin_memcpy(&self->base_0x8, &other->base_0x8, sizeof(UnkPair16));
    self->vtable = &g_unk_7101a8ad38[2];
    __builtin_memcpy(&self->field_0x18, &other->field_0x18, 2 * sizeof(UnkPair16));
    return self;
}

// Declared as the reflection traits (src/reflection/CShotVariableAngleLaunchConfig.cpp) declare them.
void fn_71009ff5b0(void* dst, const void* src) {
    CopyConfig(static_cast<UnkVariableAngleLaunchConfig*>(dst), static_cast<const UnkVariableAngleLaunchConfig*>(src));
}
void fn_71009ff5ec(void* dst, const void* src) {
    CopyConfig(static_cast<UnkVariableAngleLaunchConfig*>(dst), static_cast<const UnkVariableAngleLaunchConfig*>(src));
}
