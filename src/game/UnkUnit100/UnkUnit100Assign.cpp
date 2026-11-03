// From unit 100 of tools/tu_map.py (0x71001043e4-0x7100104ef4): the copy assignment of the
// reflected value described by fn_710010443c: a word at +0x0 and an array of 12-byte entries at
// +0x8 (data, count, capacity, the allocator's flags at +0x20 and the allocator at +0x28). The
// array grows to the source's capacity when it is smaller, then the entries are copied bitwise.
// Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

namespace {
struct UnkUnit100Entry {
    u32 words[3];
};

struct UnkUnit100Value {
    u32 field_0x0;
    UnkUnit100Entry* data;  // 0x8
    u32 count;              // 0x10
    u32 capacity;           // 0x14
    u32 step;               // 0x18
    bool field_0x1c;
    u32 flags;                     // 0x20
    base::UnkAllocator* allocator;  // 0x28

    __attribute__((always_inline)) UnkUnit100Value& operator=(const UnkUnit100Value& other) {
        if (this != &other) {
            field_0x0 = other.field_0x0;
            count = 0;
            u32 cap = other.capacity;
            UnkUnit100Entry* p;
            if (capacity < cap) {
                p = static_cast<UnkUnit100Entry*>(allocator->vf_0x40((u64)cap * sizeof(UnkUnit100Entry), 8, flags));
                if (data) {
                    __builtin_memcpy(p, data, (u64)count * sizeof(UnkUnit100Entry));
                    allocator->vf_0x48(data);
                }
                data = p;
                capacity = cap;
            } else {
                p = data;
            }
            __builtin_memcpy(p, other.data, (u64)other.count * sizeof(UnkUnit100Entry));
            count = other.count;
        }
        return *this;
    }
};
static_assert(sizeof(UnkUnit100Value) == 0x30, "UnkUnit100Value layout");
}  // namespace

void* fn_7100104cfc(void* dst, const void* src) {
    return &(*static_cast<UnkUnit100Value*>(dst) = *static_cast<const UnkUnit100Value*>(src));
}
