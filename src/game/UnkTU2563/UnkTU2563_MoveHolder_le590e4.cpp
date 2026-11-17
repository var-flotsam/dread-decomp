// Unit 2563 (CMinimapData hint): move constructor of a holder that is empty (0), holds a keyed record
// (1), or refers to a key and a record held elsewhere (2). Both non-empty kinds become an owned
// record: the string ids and the buffer are moved out of the source. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

using base::global::UnkStringData;

// A string id that is moved from: the source gets the empty string.
using UnkMovedId_e590e4 = UnkMovedStrId;

struct UnkBuffer_e590e4 {
    void* data;
    u32 size;
    u32 capacity;
};

struct UnkRecord_e590e4 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u32 field_0x20;
    u32 field_0x24;
    UnkMovedId_e590e4 field_0x28;
    UnkMovedId_e590e4 field_0x30;
    UnkBuffer_e590e4 field_0x38;
    u32 field_0x48;
    bool field_0x4c;
    u32 field_0x50;
    u64 field_0x58;

    UnkRecord_e590e4(UnkRecord_e590e4&& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), field_0x10(other.field_0x10), field_0x14(other.field_0x14),
          field_0x18(other.field_0x18), field_0x1c(other.field_0x1c), field_0x20(other.field_0x20),
          field_0x24(other.field_0x24), field_0x28(static_cast<UnkMovedId_e590e4&&>(other.field_0x28)),
          field_0x30(static_cast<UnkMovedId_e590e4&&>(other.field_0x30)),
          field_0x38{other.field_0x38.data, other.field_0x38.size, other.field_0x38.capacity},
          field_0x48(other.field_0x48), field_0x4c(other.field_0x4c), field_0x50(other.field_0x50),
          field_0x58(other.field_0x58) {
        other.field_0x38.data = nullptr;
        other.field_0x38.size = 0;
        other.field_0x38.capacity = 0;
    }
};

struct UnkKeyed_e590e4 {
    UnkMovedId_e590e4 key;
    UnkRecord_e590e4 record;

    UnkKeyed_e590e4(UnkMovedId_e590e4&& key, UnkRecord_e590e4&& record)
        : key(static_cast<UnkMovedId_e590e4&&>(key)), record(static_cast<UnkRecord_e590e4&&>(record)) {}
    UnkKeyed_e590e4(UnkKeyed_e590e4&& other)
        : key(static_cast<UnkMovedId_e590e4&&>(other.key)), record(static_cast<UnkRecord_e590e4&&>(other.record)) {}
};

struct UnkRefs_e590e4 {
    UnkMovedId_e590e4* key;
    UnkRecord_e590e4* record;
};

struct UnkHolder_e590e4 {
    union {
        UnkKeyed_e590e4 value;
        UnkRefs_e590e4 refs;
    };
    u32 kind;  // 0x68
};

void fn_7100e590e4(UnkHolder_e590e4* self, UnkHolder_e590e4* other) {
    self->kind = 0;
    switch (other->kind) {
    case 1:
        self->kind = 1;
        new (&self->value) UnkKeyed_e590e4(static_cast<UnkKeyed_e590e4&&>(other->value));
        break;
    case 2:
        self->kind = 1;
        new (&self->value) UnkKeyed_e590e4(static_cast<UnkMovedId_e590e4&&>(*other->refs.key),
                                          static_cast<UnkRecord_e590e4&&>(*other->refs.record));
        break;
    }
}
