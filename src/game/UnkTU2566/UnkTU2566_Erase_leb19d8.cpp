// CTunableMinimapManager's unit (2566): erases one element of an array of vectors of id-keyed entries with a
// string id per element alongside: the element is freed and the last one moved into its place. Returns the
// element's slot. Names and layouts are placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

// A string id and an 8-byte value.
struct UnkIdEntry_leb19d8 {
    base::UnkStrIdMember id;
    u64 value;
};
using UnkIdEntryVec_leb19d8 = base::global::CRntVector<UnkIdEntry_leb19d8>;

struct UnkIdVecArray_leb19d8 {
    UnkIdEntryVec_leb19d8* records;
    base::global::CStrId* ids;
    u32 count;
    u32 capacity;
};

void* fn_7100eb19d8(char* array, void* it) {
    UnkIdVecArray_leb19d8* self = reinterpret_cast<UnkIdVecArray_leb19d8*>(array);
    u32 index = static_cast<UnkIdEntryVec_leb19d8*>(it) - self->records;
    self->records[index].Free();
    self->ids[index].~CStrId();
    if (self->count - 1 != index) {
        new (&self->records[index])
            UnkIdEntryVec_leb19d8(static_cast<UnkIdEntryVec_leb19d8&&>(self->records[self->count - 1]));
        self->records[self->count - 1].Free();
        base::global::CStrId::MoveConstructReloading(&self->ids[index], self->ids[self->count - 1]);
        self->ids[self->count - 1].~CStrId();
    }
    --self->count;
    return &self->records[index];
}
