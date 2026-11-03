// CBossRushSurvivalConfiguration's unit (tools/tu_map.py): the constructor and move constructor of a
// record of four arrays with allocator flags 0xe (step 10, the default allocator; the second built out
// of line, fn_7100dcf890, with 5) and a last word. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_7100dcf890(void* self, u32 a, u32 flags);

struct UnkSurvivalArray_b42a3 {
    void* data;
    u32 count;
    u32 capacity;
    u32 field_0x10;
    u32 field_0x14;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Init() {
        data = nullptr;
        count = 0;
        capacity = 0;
        field_0x10 = 0;
        field_0x14 = 0;
        step = 10;
        flags = 0xe;
        allocator = fn_7100083990();
    }

    // Takes the other's storage and settings, leaving it empty.
    __attribute__((always_inline)) void MoveFrom(UnkSurvivalArray_b42a3& other) {
        data = other.data;
        // The count and the capacity are copied as one word.
        __builtin_memcpy(&count, &other.count, sizeof(count) + sizeof(capacity));
        field_0x10 = other.field_0x10;
        field_0x14 = other.field_0x14;
        step = other.step;
        flags = other.flags;
        allocator = other.allocator;
        other.field_0x10 = 0;
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

struct UnkSurvivalConfiguration_b42a3 {
    UnkSurvivalArray_b42a3 a;
    UnkSurvivalArray_b42a3 b;  // built out of line (fn_7100dcf890)
    UnkSurvivalArray_b42a3 c;
    UnkSurvivalArray_b42a3 d;
    u64 field_0xa0;
};

void fn_7100dcfce8(UnkSurvivalConfiguration_b42a3* self) {
    self->a.Init();
    fn_7100dcf890(&self->b, 5, 0xe);
    self->c.Init();
    self->d.Init();
    self->field_0xa0 = 0;
}

// The move constructor: each array takes the other's, then the last word is copied.
void fn_7100dcfd9c(UnkSurvivalConfiguration_b42a3* self, UnkSurvivalConfiguration_b42a3* other) {
    self->a.MoveFrom(other->a);
    self->b.MoveFrom(other->b);
    self->c.MoveFrom(other->c);
    self->d.MoveFrom(other->d);
    self->field_0xa0 = other->field_0xa0;
}
