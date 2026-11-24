// CForcedMovementAreaComponent's unit (1256): erases one element of an array of vectors of refcounted
// pointers with a string id per element alongside: the element is freed and the last one moved into its
// place. Returns the element's slot. Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneCalls.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkRefTag_l7ec684;
using UnkRefPtrVec_l7ec684 = base::global::CRntVector<base::UnkRefOwner<UnkRefTag_l7ec684>>;

struct UnkIdVecArray_l7ec684 {
    UnkRefPtrVec_l7ec684* records;
    base::global::CStrId* ids;
    u32 count;
    u32 capacity;
};

void* CForcedMovementAreaComponent_EraseIdRefVecEntry(char* array, void* it) {
    UnkIdVecArray_l7ec684* self = reinterpret_cast<UnkIdVecArray_l7ec684*>(array);
    u32 index = static_cast<UnkRefPtrVec_l7ec684*>(it) - self->records;
    self->records[index].Free();
    self->ids[index].~CStrId();
    if (self->count - 1 != index) {
        new (&self->records[index])
            UnkRefPtrVec_l7ec684(static_cast<UnkRefPtrVec_l7ec684&&>(self->records[self->count - 1]));
        self->records[self->count - 1].Free();
        base::global::CStrId::MoveConstructReloading(&self->ids[index], self->ids[self->count - 1]);
        self->ids[self->count - 1].~CStrId();
    }
    --self->count;
    return &self->records[index];
}
