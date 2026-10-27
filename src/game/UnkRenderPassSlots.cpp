// From unit 314 of tools/tu_map.py (0x710021812c-0x71002182bc; its static initialiser only builds
// the header ids): the constructor of the 0x48-byte object the render pass list's unit creates
// on first use (fn_71001fc6d4 keeps it in g_unk_716e598120): an allocator, a word, and an array of
// 11 slots, null at first. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkRenderPassSlots {
    base::UnkAllocator* allocator;
    u32 field_0x8;
    u32 field_0xc;
    base::UnkVector<void*> slots;
    u64 field_0x38;
    u64 field_0x40;

    __attribute__((always_inline)) UnkRenderPassSlots();
};
static_assert(sizeof(UnkRenderPassSlots) == 0x48, "UnkRenderPassSlots layout");

// Sets the array to `n` elements: new ones are null; the array only grows to exactly `n`.
static inline void Resize(base::UnkVector<void*>& v, u32 n) {
    u32 count = v.count;
    if (count < n) {
        v.Reserve(n);
        __builtin_memset(&v.data[v.count], 0, (u64)(n - v.count) * sizeof(void*));
    } else if (count == n) {
        return;
    }
    v.count = n;
}

inline UnkRenderPassSlots::UnkRenderPassSlots()
    : allocator(fn_7100083990()), field_0x8(0xe), field_0xc(0), slots(0xe), field_0x38(0), field_0x40(0) {
    Resize(slots, 11);
}

// The constructor, out of line (the caller allocates the object).
void fn_710021812c(UnkRenderPassSlots* self) {
    new (self) UnkRenderPassSlots();
}
