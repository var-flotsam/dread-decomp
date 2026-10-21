// Copy constructor of a polymorphic record of seven string ids. fn_7100968b6c is an identical
// copy.
#include <new>

#include "types.h"

// The pooled string data, seen here with a plain (type-based alias) reference count: the pool
// is loaded once for all seven copies.
struct UnkStringData {
    u8 unk_0x0[0x8];
    s32 ref_count;
};

struct UnkStringPool {
    u8 unk_0x0[0x10];
    UnkStringData empty;
};

extern UnkStringPool* g_unk_7101d47108;

struct UnkId {
    UnkStringData* data;

    UnkId(const UnkId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
};

struct UnkIdRecord {
    virtual void vf0();
    UnkId field_0x8;
    UnkId field_0x10;
    UnkId field_0x18;
    UnkId field_0x20;
    UnkId field_0x28;
    UnkId field_0x30;
    UnkId field_0x38;

    UnkIdRecord(const UnkIdRecord& other) = default;
};

static inline __attribute__((always_inline)) void CopyRecord(UnkIdRecord* self, const UnkIdRecord* other) {
    new (self) UnkIdRecord(*other);
}

void fn_7100968a84(UnkIdRecord* self, const UnkIdRecord* other) {
    CopyRecord(self, other);
}

void fn_7100968b6c(UnkIdRecord* self, const UnkIdRecord* other) {
    CopyRecord(self, other);
}
