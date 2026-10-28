// Move constructor of a holder that is empty (0), holds a keyed record (1), or refers to a key
// and a record held elsewhere (2). Both non-empty kinds become an owned record: the key and the
// plain fields are copied, the string id is copied, and the buffer is moved out of the source.
#include "base/global/CStrId.h"
#include "types.h"

#include <new>
#include <utility>

using base::global::CStrId;

struct UnkBuffer_710075de48 {
    void* data;
    u32 size;
    u32 capacity;

};

struct UnkRecordBase_710075de48 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    CStrId field_0x10;
    UnkBuffer_710075de48 field_0x18;
    u32 field_0x28;
    bool field_0x2c;
    u32 field_0x30;
    u64 field_0x38;

    UnkRecordBase_710075de48(UnkRecordBase_710075de48&& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0x10(other.field_0x10), field_0x18{other.field_0x18.data, other.field_0x18.size, other.field_0x18.capacity},
          field_0x28(other.field_0x28),
          field_0x2c(other.field_0x2c), field_0x30(other.field_0x30), field_0x38(other.field_0x38) {
        other.field_0x18.data = nullptr;
        other.field_0x18.size = 0;
        other.field_0x18.capacity = 0;
    }
};

struct UnkRecord_710075de48 : UnkRecordBase_710075de48 {
    bool field_0x40;

    UnkRecord_710075de48(UnkRecord_710075de48&& other)
        : UnkRecordBase_710075de48(std::move(other)), field_0x40(other.field_0x40) {}
};

struct UnkKeyed_710075de48 {
    u32 key;
    UnkRecord_710075de48 record;

    UnkKeyed_710075de48(u32 key, UnkRecord_710075de48&& record) : key(key), record(std::move(record)) {}
    UnkKeyed_710075de48(UnkKeyed_710075de48&& other) : key(other.key), record(std::move(other.record)) {}
};

struct UnkRefs_710075de48 {
    u32* key;
    UnkRecord_710075de48* record;
};

struct UnkHolder_710075de48 {
    union {
        UnkKeyed_710075de48 value;
        UnkRefs_710075de48 refs;
    };
    u32 kind;
};

void fn_710075de48(UnkHolder_710075de48* self, UnkHolder_710075de48* other) {
    self->kind = 0;
    switch (other->kind) {
    case 1:
        self->kind = 1;
        new (&self->value) UnkKeyed_710075de48(std::move(other->value));
        break;
    case 2:
        self->kind = 1;
        new (&self->value) UnkKeyed_710075de48(*other->refs.key, std::move(*other->refs.record));
        break;
    }
}
