// base::spatial::CPolygonCollection2D's unit (unit 109; see CPolygonCollection2D.cpp): whether any
// of the collection's 0x48-byte polygons, tried from the last to the first, answers the test
// fn_7100117aa8 for a query and a distance. Names and layouts are placeholders.
#include "types.h"

struct UnkPolygon109 {
    u8 data[0x48];
};

bool fn_7100117aa8(UnkPolygon109* polygon, void* query, f32 distance);

struct UnkPolygons109 {
    UnkPolygon109* data;
    u32 count;
};

bool fn_71001251e0(UnkPolygons109* self, void* query, f32 distance) {
    u64 i = self->count;
    bool found;
    do {
        if (i == 0)
            return false;
        u32 index = (u32)i - 1;
        found = fn_7100117aa8(&self->data[index], query, distance);
        --i;
    } while (!found);
    return true;
}
