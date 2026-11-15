// UnkTU180: copies up to `size` bytes of `data` into the current frame's buffer of one uniform block
// and marks it changed. Names and layouts are placeholders.
#include "game/UnkTU172.h"
#include "game/UnkTU180.h"
#include "types.h"

extern "C" void* memcpy(void* dst, const void* src, u64 n);

struct UnkUniformBlock {
    u8 pad_0x00[0x38];
    u64 field_0x38;     // size in bytes
    void* buffers[10];  // one per frame
    u8 pad_0x90[0x40];
    u8 field_0xd0;      // changed
    u8 pad_0xd1[7];
};
static_assert(sizeof(UnkUniformBlock) == 0xd8);

struct UnkUniformTable {
    u8 pad_0x00[0xb0];
    UnkUniformBlock* blocks;
};

struct UnkUniformTables {
    UnkUniformTable* tables[3];
};

void fn_710015dd00(UnkUniformTables* self, u32 kind, u64 handle, const void* data, u32 maxSize) {
    if (!handle)
        return;
    if (!self)
        return;
    u64 index = handle + 0xffff0001ull;
    UnkUniformTable* table;
    switch (kind) {
    case 0:
        table = self->tables[0];
        break;
    case 1:
        table = self->tables[1];
        break;
    case 2:
        table = self->tables[2];
        break;
    default:
        table = nullptr;
        break;
    }
    UnkUniformBlock* block = &table->blocks[(u32)index];
    u64 size = block->field_0x38;
    if (!size)
        return;
    u32 n = (u32)size;
    if (n > maxSize)
        n = maxSize;
    memcpy(block->buffers[fn_7100153da0()], data, n);
    block->field_0xd0 = 1;
    g_unk_716e2a3450 = 1;
}
