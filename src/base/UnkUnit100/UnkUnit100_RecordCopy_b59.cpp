// From unit 100 of tools/tu_map.py (0x71001043e4-0x7100104ef4): the copy constructor (reflection
// slot 0x30 of class_descriptors_00.cpp) of a reflected 0x30-byte record: a word, then an array
// (base::UnkVector's layout) of 12-byte trivially copied elements, grown to the source's capacity
// and copied bitwise. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkUnit100Elem_b59 {
    u32 words[3];
};

struct UnkUnit100Array_b59 {
    UnkUnit100Elem_b59* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkUnit100Array_b59(const UnkUnit100Array_b59& o)
        : data(nullptr), count(0), capacity(0), step(o.step), field_0x14(o.field_0x14), flags(o.flags),
          allocator(o.allocator) {
        u32 cap = o.capacity;
        UnkUnit100Elem_b59* p;
        if (capacity < cap) {
            p = static_cast<UnkUnit100Elem_b59*>(allocator->vf_0x40((u64)cap * sizeof(UnkUnit100Elem_b59), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(UnkUnit100Elem_b59));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = cap;
        } else {
            p = data;
        }
        __builtin_memcpy(p, o.data, (u64)o.count * sizeof(UnkUnit100Elem_b59));
        count = o.count;
    }
};

struct UnkUnit100Record_b59 {
    u32 field_0x0;
    UnkUnit100Array_b59 entries;
};
static_assert(sizeof(UnkUnit100Record_b59) == 0x30, "UnkUnit100Record_b59 layout");

void fn_7100104b90(void* dst, const void* src) {
    new (dst) UnkUnit100Record_b59(*static_cast<const UnkUnit100Record_b59*>(src));
}
