// Unit 2640: assigns a copy of a group to element `index` of an array of 0x78-byte groups (a word, an array
// of path records, a run of plain fields and an array of u64 ids), growing the array to the next multiple of
// its step and resizing it to index + 1 first. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU2640.h"
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

struct UnkRec78Init_b0212 {
    u64 field_0x0;
    base::global::CRntVector<u8> items_0x8{0xf};
    u64 field_0x30 = 0;
    f32 scale_x = 1.0f;
    f32 scale_y = 1.0f;
    u64 field_0x40 = 0;
    u32 field_0x48;
    u8 field_0x4c = 0;
    base::global::CRntVector<u8> items_0x50{0xf};
};

void fn_710100147c(UnkElemArray<UnkElem78>* self, u32 capacity);
void fn_7101002358(UnkElemArray<UnkRec78Init_b0212>* self, u32 n);

// Copies an id array over: the array grows to the other's capacity first.
static inline void AssignIds(base::global::CRntVector<u64>* dst, const base::global::CRntVector<u64>* src) {
    dst->count = 0;
    u32 n = src->capacity;
    u64* p;
    if (dst->capacity < n) {
        p = static_cast<u64*>(dst->allocator->vf_0x40((u64)n * sizeof(u64), 8, dst->flags));
        if (dst->data) {
            __builtin_memcpy(p, dst->data, (u64)dst->count * sizeof(u64));
            dst->allocator->vf_0x48(dst->data);
        }
        dst->data = p;
        dst->capacity = n;
    } else {
        p = dst->data;
    }
    __builtin_memcpy(p, src->data, (u64)src->count * sizeof(u64));
    dst->count = src->count;
}

struct UnkTU2640Group {
    u32 field_0x0;
    union {
        base::global::CRntVector<u8> entries;  // 0x8
    };
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;
    union {
        base::global::CRntVector<u64> ids;  // 0x50
    };

    // The word, the entries (fn_7100ffbad8), the run of fields, and the ids copied over (the array
    // grows to the other's capacity first).
    UnkTU2640Group& operator=(const UnkTU2640Group& other) {
        field_0x0 = other.field_0x0;
        fn_7100ffbad8(reinterpret_cast<UnkPartFfca00*>(&entries), reinterpret_cast<const UnkPartFfca00*>(&other.entries));
        __builtin_memcpy(&field_0x30, &other.field_0x30, 0x1d);
        if (this != &other)
            AssignIds(&ids, &other.ids);
        return *this;
    }
};
static_assert(sizeof(UnkTU2640Group) == 0x78, "UnkTU2640Group layout");

void fn_7101001088(UnkElemArray<UnkTU2640Group>* self, const u32* index, const UnkTU2640Group* value) {
    u32 i = *index;
    if (self->field_0x14 && self->count <= i) {
        u32 grown = self->capacity;
        if (grown <= i && self->step != 0)
            grown = (i / self->step + 1) * self->step;
        fn_710100147c(reinterpret_cast<UnkElemArray<UnkElem78>*>(self), grown);
        fn_7101002358(reinterpret_cast<UnkElemArray<UnkRec78Init_b0212>*>(self), i + 1);
    }
    self->data[i] = *value;
}
