// Unit 2640: appends a copy of a group (a word, an array of path records, a run of plain fields and an array of
// u64 ids) to an array of 0x78-byte groups, reserving through the unit's out-of-line Reserve (fn_710100147c) as
// base::global::CRntVector::PushBack does. Names and layouts are placeholders.
#include <new>

#include "../UnkUnit2601/UnkUnit2601_VecCopy_b56.h"
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkElem78 {
    u8 unk_0x0[0x78];
};

template <class Elem>
struct UnkElemArray {
    Elem* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710100147c(UnkElemArray<UnkElem78>* self, u32 capacity);

struct UnkTU2640Group {
    u32 field_0x0;
    UnkPathRecordVec_b56 entries;  // 0x8
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;
    union {
        base::global::CRntVector<u64> ids;  // 0x50
    };

    UnkTU2640Group(const UnkTU2640Group& o) : field_0x0(o.field_0x0) {
        fn_7100f85f00(&entries, &o.entries);
        __builtin_memcpy(&field_0x30, &o.field_0x30, 0x1d);
        ids.data = nullptr;
        __builtin_memset(&ids.count, 0, 8);  // count and capacity
        ids.step = o.ids.step;
        ids.field_0x14 = o.ids.field_0x14;
        ids.flags = o.ids.flags;
        ids.allocator = o.ids.allocator;
        u32 capacity = o.ids.capacity;
        u64* p = nullptr;
        if (capacity) {
            p = static_cast<u64*>(ids.allocator->vf_0x40((u64)capacity * sizeof(u64), 8, ids.flags));
            if (ids.data) {
                __builtin_memcpy(p, ids.data, (u64)ids.count * sizeof(u64));
                ids.allocator->vf_0x48(ids.data);
            }
            ids.data = p;
            ids.capacity = capacity;
        }
        __builtin_memcpy(p, o.ids.data, (u64)o.ids.count * sizeof(u64));
        ids.count = o.ids.count;
    }
};
static_assert(sizeof(UnkTU2640Group) == 0x78, "UnkTU2640Group layout");

void fn_7101001694(UnkElemArray<UnkElem78>* self, const UnkElem78* value) {
    u32 n = self->count;
    u32 c = self->capacity;
    if (c <= n && self->step != 0)
        c = (n / self->step + 1) * self->step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    fn_710100147c(self, c);
    UnkTU2640Group* slot = reinterpret_cast<UnkTU2640Group*>(&self->data[self->count]);
    new (slot) UnkTU2640Group(*reinterpret_cast<const UnkTU2640Group*>(value));
    ++self->count;
}
