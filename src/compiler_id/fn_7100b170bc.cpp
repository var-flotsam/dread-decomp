// Returns (first word of the last 8-byte element) + 1, or 0 when empty.
// The count is signed but the index is used unsigned (zero-extended).
#include "types.h"

struct UnkStruct_0x8 {
    s32 field_0x0;
    s32 field_0x4;
};

struct UnkStruct_0x50 {
    u8 unk_0x0[0x40];
    UnkStruct_0x8* data;
    s32 count;
};

s32 fn_7100b170bc(UnkStruct_0x50* self) {
    s32 last = self->count - 1;
    if (last < 0)
        return 0;
    return self->data[(u32)last].field_0x0 + 1;
}
