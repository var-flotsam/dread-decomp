// Unit 2640: assigns a copy of a group to element `index` of an array of 0x78-byte groups (a 0x50-byte record
// and an array of u64 ids), growing the array to the next multiple of its step and resizing it to index + 1
// first. Names and layouts are placeholders.
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

void fn_7101002358(UnkElemArray<UnkRec78Init_b0212>* self, u32 n);

struct UnkPartFfca00 {
    u64 unk_0x0[5];
    UnkPartFfca00& operator=(const UnkPartFfca00& other) {
        fn_7100ffbad8(this, &other);
        return *this;
    }
};

struct UnkRecordFfca00 {
    u32 field_0x0;
    UnkPartFfca00 part_0x8;
    u64 field_0x30;
    u64 field_0x38;
    u64 field_0x40;
    u32 field_0x48;
    bool field_0x4c;
};

// An array of u64 ids whose copy assignment grows it to the other's capacity first, then copies the ids over.
struct UnkTU2640IdArray : base::global::CRntVector<u64> {
    UnkTU2640IdArray& operator=(const UnkTU2640IdArray& other) {
        if (this != &other) {
            count = 0;
            u32 n = other.capacity;
            u64* p;
            if (capacity < n) {
                p = static_cast<u64*>(allocator->vf_0x40((u64)n * sizeof(u64), 8, flags));
                if (data) {
                    __builtin_memcpy(p, data, (u64)count * sizeof(u64));
                    allocator->vf_0x48(data);
                }
                data = p;
                capacity = n;
            } else {
                p = data;
            }
            __builtin_memcpy(p, other.data, (u64)other.count * sizeof(u64));
            count = other.count;
        }
        return *this;
    }
};

struct UnkTU2640Group {
    UnkRecordFfca00 record;
    UnkTU2640IdArray ids;  // 0x50
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
