// Unit 811 (CEmmyAIComponent hint): appends a copy of a 0x48-byte record to a fixed array of eight, reports
// a status code (1 added, 2 full, 3 when this filled the last slot). Names and layouts are placeholders.
#include "base/UnkNoArg.h"
#include "types.h"

void fn_7100331c7c(void* self, UnkNoArg other);

struct UnkItem_5668a8 {
    u8 bytes[0x48];
};

struct UnkAppendResult_5668a8 {
    u32 code;
    u32 field_0x4;
    u64 field_0x8;
    u64 field_0x10;
    UnkAppendResult_5668a8() {}
};

struct UnkArray_5668a8 {
    u8 pad_0x0[8];
    u32 count;
    UnkItem_5668a8 items[8];
};

void fn_7100566930(UnkItem_5668a8* items, UnkItem_5668a8* end, UnkNoArg* tag);

UnkAppendResult_5668a8 fn_71005668a8(UnkArray_5668a8* self) {
    UnkAppendResult_5668a8 result;
    u32 code;
    s32 room = 8 - self->count;
    if (room >= 1) {
        fn_7100331c7c(&self->items[self->count], UnkNoArg());
        ++self->count;
        code = room == 1 ? 3 : 1;
    } else {
        code = 2;
    }
    result.code = code;
    UnkNoArg tag;
    fn_7100566930(self->items, &self->items[self->count], &tag);
    return result;
}
