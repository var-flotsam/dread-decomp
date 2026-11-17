// Unit 108: copies a polygon into a polygon array at an index, shifting the polygons after it up.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU108.h"
#include "game/UnkXformNode.h"
#include "types.h"

// base::global::CRntVector<UnkVec3f>'s layout with plain members, so the polygon's copy constructor
// leaves it to fn_710011d8b4.
struct UnkVertexArray_l11f4ac {
    UnkVec3f* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkVertexArray_l11f4ac() {}
    UnkVertexArray_l11f4ac(UnkVertexArray_l11f4ac&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    void Free() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

// A polygon: its vertex array, then the cached length, flags, bounds and winding state.
struct UnkPolygon_l11f4ac {
    UnkVertexArray_l11f4ac vertices;
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

    // Copies other's vertices; the bounds are recomputed on next use.
    UnkPolygon_l11f4ac(const UnkPolygon_l11f4ac& other) {
        fn_710011d8b4(reinterpret_cast<UnkPart0*>(&vertices), reinterpret_cast<const UnkPart0*>(&other.vertices));
        field_0x28 = other.field_0x28;
        closed = other.closed;
        dirty = true;
        min_x = 3.402823466e+38f;
        min_y = 3.402823466e+38f;
        max_x = -3.402823466e+38f;
        max_y = -3.402823466e+38f;
        state = other.state;
    }
    // Takes over other's vertices; the bounds are recomputed on next use.
    UnkPolygon_l11f4ac(UnkPolygon_l11f4ac&& other)
        : vertices(static_cast<UnkVertexArray_l11f4ac&&>(other.vertices)),
          field_0x28(other.field_0x28), closed(other.closed), dirty(true), min_x(3.402823466e+38f),
          min_y(3.402823466e+38f), max_x(-3.402823466e+38f), max_y(-3.402823466e+38f), state(other.state) {}
    ~UnkPolygon_l11f4ac() { vertices.Free(); }
};
static_assert(sizeof(UnkPolygon_l11f4ac) == 0x48, "UnkPolygon_l11f4ac layout");

struct UnkPolygonArray_l11f4ac {
    UnkPolygon_l11f4ac* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // Out of line: grows the array to at least n elements.
    void Reserve(u32 n) asm("fn_710011f1ec");
};

void fn_710011f4ac(UnkPolygonArray_l11f4ac* self, const UnkPolygon_l11f4ac* value, u32 index) {
    u32 n = self->count;
    u32 cap = self->capacity;
    if (n == index) {
        if (cap <= n && self->step != 0)
            cap = (index / self->step + 1) * self->step;
        // Appending past the largest signed capacity hangs, as the original does.
        if ((s32)cap <= (s32)index)
            for (;;) {
            }
        self->Reserve(cap);
        new (&self->data[self->count]) UnkPolygon_l11f4ac(*value);
    } else {
        if (cap <= n && self->step != 0)
            cap = (n / self->step + 1) * self->step;
        self->Reserve(cap);
        for (u32 i = self->count - 1; i >= index; --i) {
            UnkPolygon_l11f4ac* q = &self->data[i];
            new (q + 1) UnkPolygon_l11f4ac(static_cast<UnkPolygon_l11f4ac&&>(*q));
            self->data[i].~UnkPolygon_l11f4ac();
            if (i == 0)
                break;
        }
        new (&self->data[index]) UnkPolygon_l11f4ac(*value);
    }
    ++self->count;
}
