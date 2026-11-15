// Unit 83 (base::reflection): erases a 12-byte entry from an array by index, moving later ones down.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry83 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};

struct UnkArray83 {
    UnkEntry83* data;
    u32 count;  // 0x8
};

void fn_71000cb7ac(UnkArray83* self, const u32* index) {
    u64 offset = (u64)*index * 3;
    u32 i = offset / 3;
    if (i < self->count) {
        for (u64 j = i; j < self->count - 1; ++j)
            self->data[j] = self->data[j + 1];
        --self->count;
    }
}
