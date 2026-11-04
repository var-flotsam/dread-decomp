// tools/tu_map.py unit 232 (no class hint): the seven 16-bit fields of a 14-byte record by index,
// when the table is loaded and enabled and the index is in range. Names and layouts are placeholders.
#include "types.h"

struct UnkRecord232_b66a1 {
    u16 values[7];
};

struct UnkRecordTable232_b66a1 {
    u8 pad_0x0[0x20];
    u32 count;  // 0x20
    u8 pad_0x24[0x4];
    UnkRecord232_b66a1* records;  // 0x28
};

struct UnkRecordOwner232_b66a1 {
    bool enabled;  // 0x0
    u8 pad_0x1[0xf];
    UnkRecordTable232_b66a1* table;  // 0x10
};

void fn_71001ab3dc(UnkRecordOwner232_b66a1* self, u32 index, u16* a, u16* b, u16* c, u16* d, u16* e, u16* f,
                   u16* g) {
    UnkRecordTable232_b66a1* table = self->table;
    if (table == nullptr || !self->enabled || index >= table->count) {
        return;
    }
    const UnkRecord232_b66a1& r = table->records[index];
    *a = r.values[0];
    *b = r.values[1];
    *c = r.values[2];
    *d = r.values[3];
    *e = r.values[4];
    *f = r.values[5];
    *g = r.values[6];
}
