// Unit SCameraRailNode: erases the 8-byte entry at an index (read through a pointer) from an array, moving the
// later entries down one. Names and layouts are placeholders.
#include "types.h"

struct UnkEntry8 {
    u32 field_0x0;
    u32 field_0x4;
    UnkEntry8& operator=(const UnkEntry8& o) {
        field_0x0 = o.field_0x0;
        field_0x4 = o.field_0x4;
        return *this;
    }
};

struct UnkEntryArray8 {
    UnkEntry8* data;
    u32 count;  // 0x8
};

void fn_7100c0b2f0(UnkEntryArray8* self, const u32* index) {
    u32 i = *index;
    if (self->count > i) {
        for (u64 j = i; j < self->count - 1; ++j)
            self->data[j] = self->data[j + 1];
        --self->count;
    }
}
