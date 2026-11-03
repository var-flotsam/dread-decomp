// From unit 174 of tools/tu_map.py: writing `size` raw bytes at byte `offset` of the buffer of
// src/base/UnkUnit174/UnkUnit174_Write.cpp when it is enabled and has storage (no range check).
// Names are placeholders.
#include <string.h>

#include "types.h"

struct UnkUnit174Buffer_2 {
    u8* data;
    u8 pad_0x8[0x2c];
    bool enabled;  // 0x34
};

void fn_71001542f0(UnkUnit174Buffer_2* self, u32 offset, u32 size, const void* src) {
    if (!self || !self->enabled || !self->data)
        return;
    memcpy(self->data + offset, src, size);
}
