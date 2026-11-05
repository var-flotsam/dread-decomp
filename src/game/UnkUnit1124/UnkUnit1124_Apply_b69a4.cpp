// From unit 1124 of tools/tu_map.py (0x710070368c-0x7100703cf4, no reflected class): fn_71007038f4 marks the
// object changed (+0xa8), optionally clears its three optional settings, takes each setting the argument has,
// then copies the third setting's value (or zero) into the current value at +0x58. Names and layouts are
// placeholders.
#include "types.h"

struct UnkVec3_b69a4 {
    f32 x, y, z;
};

struct UnkVec4_b69a4 {
    f32 x, y, z, w;
};

template <class T>
struct UnkOptional_b69a4 {
    T value;
    bool has;
};

struct UnkSettings_b69a4 {
    u8 pad_0x0[8];
    UnkOptional_b69a4<UnkVec3_b69a4> field_0x8;
    UnkOptional_b69a4<u32> field_0x18;
    UnkOptional_b69a4<UnkVec4_b69a4> field_0x20;
};

struct UnkUnit1124Settings_b69a4 {
    u8 pad_0x0[0x58];
    UnkVec4_b69a4 current;  // 0x58
    u8 pad_0x68[0x10];
    UnkOptional_b69a4<UnkVec3_b69a4> field_0x78;
    UnkOptional_b69a4<u32> field_0x88;
    UnkOptional_b69a4<UnkVec4_b69a4> field_0x90;
    u8 pad_0xa4[4];
    bool changed;  // 0xa8
};
static_assert(__builtin_offsetof(UnkUnit1124Settings_b69a4, changed) == 0xa8, "layout");

void fn_71007038f4(UnkUnit1124Settings_b69a4* self, const UnkSettings_b69a4* settings, bool reset) {
    self->changed = true;
    if (reset) {
        self->field_0x78.has = false;
        self->field_0x88.has = false;
        self->field_0x90.has = false;
    }
    if (settings->field_0x8.has) {
        self->field_0x78.value = settings->field_0x8.value;
        self->field_0x78.has = true;
    }
    if (settings->field_0x18.has) {
        self->field_0x88.value = settings->field_0x18.value;
        self->field_0x88.has = true;
    }
    if (settings->field_0x20.has) {
        self->field_0x90.value = settings->field_0x20.value;
        self->field_0x90.has = true;
    }
    UnkVec4_b69a4* current = &self->current;
    if (self->field_0x90.has)
        *current = self->field_0x90.value;
    else
        *current = UnkVec4_b69a4{};
}
