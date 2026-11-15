// Unit 108: erases an 8-byte entry from an array by index (read through a pointer), moving later entries down.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry8_108 {
    u32 field_0x0;
    u32 field_0x4;

    UnkEntry8_108& operator=(const UnkEntry8_108& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        return *this;
    }
};

struct UnkEntryArray8_108 {
    UnkEntry8_108* data;
    u32 count;  // 0x8
};

void fn_7100121ac4(UnkEntryArray8_108* self, const u32* index) {
    u32 idx = *index;
    if (idx < self->count) {
        for (u64 i = idx; i < self->count - 1; ++i)
            self->data[i] = self->data[i + 1];
        --self->count;
    }
}
