// game::logic::collision::CPolygonCollectionShape's unit (tools/tu_map.py unit 2515): sets the
// mode (+0x94) and, when it changed, passes it to every segment of every polygon (0x48-byte
// polygons, data +0xa8, count +0xb0; fn_710011892c per segment). A polygon has one segment per
// point when it is closed (+0x2c clear) and one fewer when it is open. Names and layouts are
// placeholders.
#include "types.h"

struct UnkPolygon_b46a3 {
    u8 pad_0x0[0x8];
    u32 count;  // 0x8
    u8 pad_0xc[0x20];
    u8 open;  // 0x2c
    u8 pad_0x2d[0x1b];

    u32 SegmentCount() const { return count - ((open == 0) & (count != 0)); }
};
static_assert(sizeof(UnkPolygon_b46a3) == 0x48, "UnkPolygon_b46a3 layout");

void fn_710011892c(UnkPolygon_b46a3* polygon, u32 segment, u32 mode);

struct UnkPolygonCollection_b46a3 {
    u8 pad_0x0[0x94];
    u32 mode;  // 0x94
    u8 pad_0x98[0x10];
    UnkPolygon_b46a3* polygons;  // 0xa8
    u32 count;                   // 0xb0
};

void fn_7100d63604(UnkPolygonCollection_b46a3* self, u32 mode) {
    if (self->mode == mode)
        return;
    self->mode = mode;
    for (UnkPolygon_b46a3* p = self->polygons, *end = self->polygons + self->count; p != end; ++p)
        for (u32 i = 0; i < p->SegmentCount(); ++i)
            fn_710011892c(p, i, mode);
}
