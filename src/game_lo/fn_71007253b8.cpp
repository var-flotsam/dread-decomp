// The element at an index of an array of 0x30-byte records (three string ids and three
// 8-byte values), growing the array to cover the index first (UnkVector::GrowAt).
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

// A string id moved by taking the pointer and leaving the source empty.
struct UnkId_71007253b8 {
    UnkStringData* data;

    UnkId_71007253b8(UnkId_71007253b8&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

struct UnkRecord_71007253b8 {
    UnkId_71007253b8 field_0x0;
    UnkId_71007253b8 field_0x8;
    UnkId_71007253b8 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;

    UnkRecord_71007253b8();   // fn_710072676c
    ~UnkRecord_71007253b8();  // fn_7100726674
    UnkRecord_71007253b8(UnkRecord_71007253b8&& other) = default;
};

// The original has GrowAt inlined; flatten keeps it from being emitted out of line.
__attribute__((flatten)) UnkRecord_71007253b8& fn_71007253b8(base::UnkVector<UnkRecord_71007253b8>* self, u32 index) {
    return self->GrowAt(index);
}
