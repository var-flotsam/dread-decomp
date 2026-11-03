// From unit 174 of tools/tu_map.py (0x7100153f40-0x7100154538, no reflected class): writing `count`
// elements at byte `offset` of a buffer whose element size comes from its format (the table
// g_unk_7101647e48 for formats 0 to 2, else 0), when the buffer is enabled, has storage and the
// range fits. Names are placeholders.
#include <string.h>

#include "types.h"

extern const u32 g_unk_7101647e48[] __attribute__((visibility("hidden")));  // element size per format

struct UnkUnit174Buffer {
    u8* data;
    u8 pad_0x8[8];
    u32 size;  // 0x10
    u8 pad_0x14[4];
    s32 format;  // 0x18
    u8 pad_0x1c[0x18];
    bool enabled;  // 0x34
};

void fn_7100154320(UnkUnit174Buffer* self, u32 offset, u32 count, const void* src) {
    if (!self || !self->enabled || !self->data)
        return;
    u32 elem = (u32)self->format <= 2 ? g_unk_7101647e48[self->format] : 0;
    u32 bytes = elem * count;
    if (offset + bytes > self->size)
        return;
    memcpy(self->data + offset, src, bytes);
}
