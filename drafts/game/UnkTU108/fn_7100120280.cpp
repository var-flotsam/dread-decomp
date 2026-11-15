// Unit 108: erases a 12-byte entry from an array by index; the index is a byte offset divided by 3.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry108 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

struct UnkEntryArray108 {
    UnkEntry108* data;
    u32 count;  // 0x8
};

struct UnkTriple108 {
    u8 bytes[3];
};

void fn_7100120280(UnkEntryArray108* self, const u32* offset) {
    u64 diff = (u64)*offset * 3;
    u32 idx = (u32)(diff / 3);
    if (idx < self->count) {
        for (u64 i = idx; i < self->count - 1; ++i)
            self->data[i] = self->data[i + 1];
        --self->count;
    }
}
