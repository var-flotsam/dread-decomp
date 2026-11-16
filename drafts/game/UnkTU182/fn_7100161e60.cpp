// Unit 182 (13.1%: the original reloads the base after storing it; the creation path is laid out last): sets one attribute slot of a graphics buffer, taking a slot record from the shared pool
// the first time; copies the data into the buffer's mapping and derives the slot's type from the format.
// Names and layouts are placeholders.
#include <string.h>

#include "game/UnkTU182.h"
#include "types.h"

struct UnkGfxBufferAttrib {
    u32 kind;
    u8 pad_0x4[4];
    const void* data;
    u32 offset;
    u32 size;
    u32 field_0x18;
    u32 type;
    u64 field_0x20;
    u32 format;
    u32 index;
};

struct UnkGfxBuffer252 {
    const void* base;
    u8* mapping;
    u8 pad_0x10[8];
    u32 attrib_count;
    u8 pad_0x1c[8];
    u32 flags;
    u32 attrib_mask;
    u8 pad_0x2c[0xc];
    UnkGfxBufferAttrib* attribs[16];
};

extern UnkGfxBufferAttrib* g_unk_716e2cae08[];  // the pool's records
extern u32 g_unk_716e34ae08;                    // how many are in use

// Takes the next free record from the pool, set to its defaults.
inline UnkGfxBufferAttrib* AllocAttrib(u32 kind) {
    UnkGfxBufferAttrib* attrib = g_unk_716e2cae08[g_unk_716e34ae08];
    attrib->kind = kind;
    attrib->data = nullptr;
    attrib->offset = 0;
    attrib->size = 0;
    attrib->field_0x18 = 0;
    attrib->type = 0x16;
    attrib->field_0x20 = 0;
    attrib->format = 0;
    attrib->index = g_unk_716e34ae08;
    return attrib;
}

void fn_7100161e60(UnkGfxBuffer252* buffer, u32 kind, u32 a, u32 offset, u32 format, const void* data, u32 size,
                   u32 b) {
    if (!buffer)
        return;
    UnkGfxBufferAttrib* attrib = buffer->attribs[kind];
    if (!attrib) {
        attrib = AllocAttrib(kind);
        buffer->attribs[kind] = attrib;
        g_unk_716e34ae08++;
        buffer->attrib_count++;
        buffer->attrib_mask |= 1 << kind;
    }
    attrib->offset = offset;
    attrib->size = size;
    if (buffer->flags & 1) {
        if (buffer->flags & 4) {
            if (data)
                buffer->base = data;
            data = static_cast<const u8*>(buffer->base) + (offset & ~3u);
        }
        attrib->data = data;
        if (buffer->mapping && data)
            memcpy(buffer->mapping + offset, data, size);
    }
    attrib->field_0x18 = a;
    attrib->format = format;
    if ((format & 0xffff) == 4) {
        attrib->type = 0x25;
        return;
    }
    switch (format >> 16) {
    case 1:
        attrib->type = 0xa;
        break;
    case 2:
        attrib->type = 0x16;
        break;
    case 3:
        attrib->type = 0x22;
        break;
    case 4:
        attrib->type = 0x2e;
        break;
    }
}
