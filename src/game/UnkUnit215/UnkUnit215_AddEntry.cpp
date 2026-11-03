// tools/tu_map.py unit 215 (no class hint): takes the next entry of a 0x84-byte record array
// (count +0x11c, records +0x120), marks its field +0x7c unset (-1) and answers its index. Names are
// placeholders.
#include "types.h"

struct UnkRecord215 {
    u8 pad_0x0[0x7c];
    s32 field_0x7c;
    u8 pad_0x80[0x4];
};
static_assert(sizeof(UnkRecord215) == 0x84, "UnkRecord215 size");

struct UnkRecordList215 {
    u8 pad_0x0[0x11c];
    u32 count;               // 0x11c
    UnkRecord215* records;   // 0x120
};

u32 fn_710019675c(UnkRecordList215* self) {
    u32 index = self->count;
    UnkRecord215* records = self->records;
    self->count = index + 1;
    records[index].field_0x7c = -1;
    return index;
}
