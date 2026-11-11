// Unit 252 of tools/tu_map.py (0x71001bdeac-0x71001c071c): writing a slot's vertex data (12 bytes
// per vertex) and its 16-byte records after it into the slot's buffer (one of five, 0x18 bytes
// apart from +0x148), when both fit (fn_71001622e0 is its size). Names and layouts are placeholders.
#include "game/UnkTU182.h"
#include "types.h"

struct UnkGfxBuffer252;

struct UnkGfxBufferHolder252 {
    u8 pad_0x0[0x8];
    UnkGfxBuffer252* buffer;
};

struct UnkGfxSlot252 {
    UnkGfxBufferHolder252* holder;
    u8 pad_0x8[0x10];
};

struct UnkGfxSlots252 {
    u8 pad_0x0[0x148];
    UnkGfxSlot252 slots[5];
};

struct UnkGfxData252 {
    const void* data;
    u32 count;
};

bool fn_71001c01fc(UnkGfxSlots252* self, u32 index, const UnkGfxData252* vertices, const UnkGfxData252* records) {
    if (index > 4)
        return false;
    UnkGfxBufferHolder252* holder = self->slots[index].holder;
    u32 vertex_size = vertices->count * 12;
    u32 record_size = records->count * 16;
    UnkGfxBuffer252* buffer = holder->buffer;
    if (record_size + vertex_size > fn_71001622e0(buffer))
        return false;
    fn_7100161d70(buffer);
    fn_7100161e60(buffer, 0, 0, 0, fn_71001622f0(3, 3), vertices->data, vertex_size, 0);
    fn_7100161e60(buffer, 5, 0, vertex_size, fn_71001622f0(4, 3), records->data, record_size, 0);
    fn_7100161dc0(buffer);
    fn_7100161d60(buffer);
    return false;
}
