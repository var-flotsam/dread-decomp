// Copy of fn_71005d6c38 (CAcidBlobsLaunchPattern's unit): the value at (row, column) of a pool's
// rows, read through fn_71004ba164; 0 out of range. Names are placeholders.
#include "types.h"

struct UnkEntry_71004cb554;
struct UnkPool_4cb554;

struct UnkPoolRow_b46a1 {
    void* data;
    u32 count;
    u8 pad_0xc[0x1c];
};

u64* fn_71004ba164(UnkPoolRow_b46a1* row, u32 column);

struct UnkPoolImpl_b46a1 {
    u8 pad_0x0[0x120];
    UnkPoolRow_b46a1* rows;
    u32 count;
};

UnkEntry_71004cb554* fn_71004ba10c(UnkPool_4cb554* pool, u32 index, u32 flag) {
    UnkPoolImpl_b46a1* self = reinterpret_cast<UnkPoolImpl_b46a1*>(pool);
    if (index < self->count && flag < self->rows[index].count)
        return reinterpret_cast<UnkEntry_71004cb554*>(*fn_71004ba164(&self->rows[index], flag));
    return 0;
}
