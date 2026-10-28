// A key/value pair whose value holds a word, a string id and a vector of string ids
// (base::UnkStrIdVec, reserved to the source's capacity and filled by appending): its copy
// constructor and its constructor from a key and a value.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdVecValue {
    u32 field_0x0;
    CStrId id;
    base::UnkStrIdVec ids;

    __attribute__((always_inline)) UnkIdVecValue(const UnkIdVecValue& other)
        : field_0x0(other.field_0x0), id(other.id), ids(other.ids) {}
};

struct UnkIdVecPair {
    u32 key;
    UnkIdVecValue value;
};

void fn_7100b5b5a8(UnkIdVecPair* self, const UnkIdVecPair* other) {
    self->key = other->key;
    new (&self->value) UnkIdVecValue(other->value);
}

void fn_7100b5b6e8(UnkIdVecPair* self, const u32* key, const UnkIdVecValue* value) {
    self->key = *key;
    new (&self->value) UnkIdVecValue(*value);
}
