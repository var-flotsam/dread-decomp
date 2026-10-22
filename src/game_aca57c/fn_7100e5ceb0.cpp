// A pair's move constructor and its constructor from a moved key and a vector: the string id
// key is taken over (the source is left holding the empty id) and the vector of string ids is
// copied (base::UnkStrIdVec).
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkMovedStrId {
    UnkStringData* data;

    __attribute__((always_inline)) UnkMovedStrId(UnkMovedStrId&& other) : data(other.data) {
        other.data = &g_unk_7101d47108->empty;
    }
};

struct UnkIdVecEntry {
    UnkMovedStrId key;
    base::UnkStrIdVec ids;

    __attribute__((always_inline)) UnkIdVecEntry(UnkIdVecEntry&& other)
        : key(static_cast<UnkMovedStrId&&>(other.key)), ids(other.ids) {}
    __attribute__((always_inline)) UnkIdVecEntry(UnkMovedStrId&& key, const base::UnkStrIdVec& ids)
        : key(static_cast<UnkMovedStrId&&>(key)), ids(ids) {}
};

void fn_7100e5ceb0(UnkIdVecEntry* self, UnkIdVecEntry* other) {
    new (self) UnkIdVecEntry(static_cast<UnkIdVecEntry&&>(*other));
}
void fn_7100e5cfd0(UnkIdVecEntry* self, UnkMovedStrId* key, const base::UnkStrIdVec* ids) {
    new (self) UnkIdVecEntry(static_cast<UnkMovedStrId&&>(*key), *ids);
}
