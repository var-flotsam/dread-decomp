// UnkTU180: copies `count` words of each of `stride` elements into a uniform range of the current
// frame's buffer (clamped to the range's size times `stride`) and marks the block, its table and
// the global flag changed. Names and layouts are placeholders.
#include "game/UnkTU172.h"
#include "game/UnkTU180.h"
#include "game/UnkUniformTables.h"
#include "types.h"

extern "C" void* memcpy(void* dst, const void* src, u64 n);

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_710015e2d0(UnkUniformTables* self, u32 kind, u64 handle, u32 count, u32 stride, const void* data) {
    u64 index = handle + 0xfffffff1u;
    if (!self || !handle)
        return;
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
    UnkUniformRange* range = &table->ranges[index & 0xffffffff];
    UnkUniformBlock* block = &table->blocks[range->block];
    if (!block->field_0x38)
        return;
    memcpy((u8*)block->buffers[fn_7100153da0()] + range->offset, data,
           count * stride * 4 < range->size * stride ? count * stride * 4 : range->size * stride);
    block->field_0xd0 = 1;
    table->field_0xb8 = 1;
    g_unk_716e2a3450 = 1;
}
