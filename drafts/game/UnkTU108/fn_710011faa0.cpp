// Unit 108: the element of a growable array of 0x48-byte polygons at an index, growing the array
// first (capacity to the next multiple of the step, then the new polygons constructed) when the
// index is past the end. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU108.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkVertexArray108 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkPolygonElem108 {
    UnkVertexArray108 vertices;
    f32 field_0x28;
    bool closed;  // 0x2c
    bool dirty;   // 0x2d
    u8 pad_0x2e[2];
    u64 bounds_min;  // 0x30, two floats
    u64 bounds_max;  // 0x38, two floats
    u32 state;       // 0x40
    u32 pad_0x44;

    UnkPolygonElem108() {
        vertices.data = nullptr;
        vertices.count = 0;
        vertices.capacity = 0;
        vertices.step = 10;
        vertices.field_0x14 = true;
        vertices.flags = 7;
        vertices.allocator = GetDefaultAllocator();
        field_0x28 = 0.0f;
        closed = false;
        dirty = true;
        bounds_min = 0x7f7fffff7f7fffffull;
        bounds_max = 0xff7fffffff7fffffull;
        state = 0;
    }
};
static_assert(sizeof(UnkPolygonElem108) == 0x48, "UnkPolygonElem108 layout");

struct UnkPolygonArray108 {
    UnkPolygonElem108* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Out of line: grows the array to at least n elements.
    void Reserve(u32 n) asm("fn_710011f1ec");
};

UnkPolygonElem108* fn_710011faa0(UnkPolygonArray108* self, u32 index) {
    if (self->field_0x14 && self->count <= index) {
        u32 capacity = self->capacity;
        if (capacity <= index && self->step != 0)
            capacity = (index / self->step + 1) * self->step;
        self->Reserve(capacity);
        u32 n = index + 1;
        self->Reserve(n);
        for (u32 i = self->count; i < n; ++i)
            new (&self->data[i]) UnkPolygonElem108();
        self->count = n;
    }
    return &self->data[index];
}
