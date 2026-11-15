// GUI::IDisplayObjectTrack: erases an 8-byte entry from an array by index, later entries moved down one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEntry1671 {
    u32 field_0x0;
    u32 field_0x4;
};

struct UnkArray1671 {
    UnkEntry1671* data;
    u32 count;  // 0x8
};

void fn_7100b19584(UnkArray1671* self, const u32* index) {
    u32 i = *index;
    if (i < self->count) {
        for (u64 k = i; k < self->count - 1; ++k)
            self->data[k] = self->data[k + 1];
        --self->count;
    }
}
