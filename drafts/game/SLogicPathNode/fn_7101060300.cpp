// Unit SLogicPathNode: erases the entry (two words) at an index and moves the later entries down one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkPathEntryArray {
    u32* data;  // two words per entry
    u32 count;  // 0x8
};

void fn_7101060300(UnkPathEntryArray* self, const u32* index) {
    u32 idx = *index;
    if (idx < self->count) {
        for (u64 i = idx; i < self->count - 1; ++i) {
            self->data[i * 2] = self->data[i * 2 + 2];
            self->data[i * 2 + 1] = self->data[i * 2 + 3];
        }
        --self->count;
    }
}
