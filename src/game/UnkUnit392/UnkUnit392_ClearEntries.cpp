#include "types.h"

struct UnkUnit392Entry {
    u8 unk_0x0[0x40];
};

void fn_71002b6920(UnkUnit392Entry* entry);

struct UnkUnit392Owner {
    u8 unk_0x0[0x18];
    UnkUnit392Entry* entries;
    u32 count;
    u8 unk_0x24[0x24];
    u32 field_0x48;
};

void fn_71002c57cc(UnkUnit392Owner* self) {
    u32 count = self->count;
    if (count != 0) {
        UnkUnit392Entry* it = self->entries;
        UnkUnit392Entry* end = it + count;
        do { fn_71002b6920(it); ++it; } while (it != end);
    }
    self->field_0x48 = 0;
}
