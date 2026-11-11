// Unit 364 of tools/tu_map.py: clears every side of every cell (fn_7100288634 for each cell index
// and side) and then applies the entries of an array of cell/side pairs (fn_7100288320), as the
// neighbouring fn_7100288904 and fn_71002885e8 do for one array. Names and layouts are placeholders.
#include "game/UnkTU364.h"
#include "types.h"

struct UnkEntry_0x4_7100288950 {
    u16 field_0x0;
    u8 field_0x2;
    u8 field_0x3;
};

struct UnkArray_0x4_7100288950 {
    UnkEntry_0x4_7100288950* data;
    u32 count;
};

struct UnkCell_7100288950 {
    u8 pad_0x0[0x8];
    u32 side_count;
};

struct UnkGenerator_7100288950 {
    u8 pad_0x0[0x8];
    u32 cell_count;
};

UnkCell_7100288950* fn_7100287de8(UnkGenerator_7100288950* self, u32 index);

void fn_7100288950(UnkGenerator_7100288950* self, const UnkArray_0x4_7100288950* entries) {
    for (u16 i = 0; i < self->cell_count; ++i) {
        for (u8 j = 0; j < fn_7100287de8(self, i)->side_count; ++j)
            fn_7100288634(self, i, j);
    }
    for (const UnkEntry_0x4_7100288950 *it = entries->data, *end = it + entries->count; it != end; ++it)
        fn_7100288320(self, it->field_0x0, it->field_0x2);
}
