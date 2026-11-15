// Unit 2661 (CCutscene::CTunableCutscene): removes the 8-byte element at an index from an array, shifting the rest down.
// Names and layouts are placeholders.
#include <string.h>

#include "types.h"

struct UnkArray2661 {
    u8* data;
    u32 count;  // 0x8
};

void fn_7101070630(UnkArray2661* self, const u32* index) {
    u32 i = *index;
    if (self->count > i) {
        u8* dst = self->data + (u64)i * 8;
        u32 tail = self->count - i - 1;
        memcpy(dst, dst + 8, (u64)tail * 8);
        self->count--;
    }
}
