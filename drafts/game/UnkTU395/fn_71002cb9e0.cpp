// Unit 395: erases a 16-byte entry from an array by index, later entries moved down one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry395 {
    u8 bytes[16];
};

struct UnkEntryArray395 {
    UnkEntry395* data;
    u32 count;  // 0x8
};

void fn_71002cb9e0(UnkEntryArray395* self, const u32* index) {
    u32 idx = *index;
    if (self->count > idx) {
        for (u64 i = idx; self->count - 1 > i; ++i)
            self->data[i] = self->data[i + 1];
        --self->count;
    }
}
