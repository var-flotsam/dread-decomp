// Copy constructor of a polymorphic record of seven string ids. fn_7100968b6c is an identical
// copy.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

struct UnkId_968a84 {
    base::global::UnkStringData* data;

    UnkId_968a84(const UnkId_968a84& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
};

struct UnkIdRecord {
    virtual void vf0();
    UnkId_968a84 field_0x8;
    UnkId_968a84 field_0x10;
    UnkId_968a84 field_0x18;
    UnkId_968a84 field_0x20;
    UnkId_968a84 field_0x28;
    UnkId_968a84 field_0x30;
    UnkId_968a84 field_0x38;

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
