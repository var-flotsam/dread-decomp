// Unit 83 (base::reflection): erases an 8-byte entry from an array by index, moving later ones down.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry83 {
    u32 field_0x0;
    u32 field_0x4;
};

struct UnkArray83 {
    UnkEntry83* data;
    u32 count;  // 0x8
};

void fn_71000cef84(UnkArray83* self, const u32* index) {
    u32 i = *index;
    if (i < self->count) {
        for (u64 j = i; j < self->count - 1; ++j)
            self->data[j] = self->data[j + 1];
        --self->count;
    }
}
