// Unit 108: moves a polygon into a polygon array at an index, shifting the polygons after it up;
// at the end it appends instead. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkXformNode.h"
#include "types.h"

// A polygon: its vertex array, then the cached length, flags, bounds and winding state.
struct UnkPolygon_l11f6a8 {
    base::global::CRntVector<UnkVec3f> vertices;
    f32 field_0x28;
    bool closed;  // 0x2c
    bool dirty;   // 0x2d
    u8 pad_0x2e[2];
    f32 min_x;  // 0x30
    f32 min_y;  // 0x34
    f32 max_x;  // 0x38
    f32 max_y;  // 0x3c
    u32 state;       // 0x40
    u32 pad_0x44;

    // Takes over other's vertices; the bounds are recomputed on next use.
    UnkPolygon_l11f6a8(UnkPolygon_l11f6a8&& other)
        : vertices(static_cast<base::global::CRntVector<UnkVec3f>&&>(other.vertices)),
          field_0x28(other.field_0x28), closed(other.closed), dirty(true), min_x(3.402823466e+38f),
          min_y(3.402823466e+38f), max_x(-3.402823466e+38f), max_y(-3.402823466e+38f), state(other.state) {}
    ~UnkPolygon_l11f6a8() { vertices.Free(); }
};
static_assert(sizeof(UnkPolygon_l11f6a8) == 0x48, "UnkPolygon_l11f6a8 layout");

struct UnkPolygonArray_l11f6a8 {
    UnkPolygon_l11f6a8* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    // Out of line: grows the array to at least n elements.
    void Reserve(u32 n) asm("fn_710011f1ec");
    // Out of line: appends value, growing the array as needed.
    void EmplaceBack(UnkPolygon_l11f6a8&& value) asm("fn_710011f880");
};

void fn_710011f6a8(UnkPolygonArray_l11f6a8* self, UnkPolygon_l11f6a8* value, u32 index) {
    u32 n = self->count;
    if (n == index) {
        self->EmplaceBack(static_cast<UnkPolygon_l11f6a8&&>(*value));
        return;
    }
    u32 cap = self->capacity;
    if (cap <= n && self->step != 0)
        cap = (n / self->step + 1) * self->step;
    self->Reserve(cap);
    for (u32 i = self->count - 1; i >= index; --i) {
        UnkPolygon_l11f6a8* q = &self->data[i];
        new (q + 1) UnkPolygon_l11f6a8(static_cast<UnkPolygon_l11f6a8&&>(*q));
        self->data[i].~UnkPolygon_l11f6a8();
        if (i == 0)
            break;
    }
    new (&self->data[index]) UnkPolygon_l11f6a8(static_cast<UnkPolygon_l11f6a8&&>(*value));
    ++self->count;
}
