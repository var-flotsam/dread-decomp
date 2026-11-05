// From unit 56 of tools/tu_map.py (0x710007bb30-0x710007e6c4): the copy constructors (reflection
// slots 0x30 and 0x38 of class_descriptors_00.cpp, the same code) of a reflected 0x38-byte block
// of memory: its allocator (the default one when the source has none), a word at +0x8, the
// allocation flags (0x16), the block and its size, an owner that frees it (+0x28, not copied),
// the alignment and four flag bytes. A copy allocates the source's size and copies the bytes.
// Names and layout are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkUnit56Block_b59 {
    base::UnkAllocator* allocator;
    u64 field_0x8;
    u32 flags;
    u8* data;
    u64 size;
    void* owner;
    u8 align;
    u8 field_0x31;
    u8 field_0x32;
    u8 field_0x33;
    u8 field_0x34;

    UnkUnit56Block_b59(const UnkUnit56Block_b59& o) {
        flags = 0x16;
        owner = nullptr;
        field_0x31 = 0;
        allocator = o.allocator;
        align = o.align;
        field_0x32 = o.field_0x32;
        field_0x33 = o.field_0x33;
        field_0x34 = 0;
        u64 n = o.size;
        u8* p;
        if (!allocator && !(allocator = fn_7100083990())) {
            p = nullptr;
            data = nullptr;
        } else {
            p = static_cast<u8*>(allocator->vf_0x40(n, align, flags));
            data = p;
            if (p) {
                size = n;
                field_0x8 = 0;
            }
        }
        __builtin_memcpy(p, o.data, o.size);
        u64 s = o.size;
        size = s;
        if (!data)
            return;
        if (s == 0 || s <= ~reinterpret_cast<u64>(data)) {
            // A null pointer, not the integer 0: the two stores stay apart, as in the original.
            *reinterpret_cast<void**>(&field_0x8) = nullptr;
            return;
        }
        field_0x8 = s;
    }
};
static_assert(sizeof(UnkUnit56Block_b59) == 0x38, "UnkUnit56Block_b59 layout");

void fn_710007dfd8(void* dst, const void* src) {
    new (dst) UnkUnit56Block_b59(*static_cast<const UnkUnit56Block_b59*>(src));
}
void fn_710007e0b8(void* dst, const void* src) {
    new (dst) UnkUnit56Block_b59(*static_cast<const UnkUnit56Block_b59*>(src));
}
