// CMinimapData's unit (2563): erases one element of an array of string-id vectors with a string id per
// element alongside: the element is freed and the last one moved into its place. Returns the element's
// slot. Names and layouts are placeholders.
#include <new>

#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkStrIdMember;
using UnkStrIdMemberVec_le5dec4 = base::global::CRntVector<UnkStrIdMember>;

struct UnkIdVecArray_le5dec4 {
    UnkStrIdMemberVec_le5dec4* records;
    base::global::CStrId* ids;
    u32 count;
    u32 capacity;
};

void* fn_7100e5dec4(char* array, void* it) {
    UnkIdVecArray_le5dec4* self = reinterpret_cast<UnkIdVecArray_le5dec4*>(array);
    u32 index = static_cast<UnkStrIdMemberVec_le5dec4*>(it) - self->records;
    self->records[index].Free();
    self->ids[index].~CStrId();
    if (self->count - 1 != index) {
        new (&self->records[index])
            UnkStrIdMemberVec_le5dec4(static_cast<UnkStrIdMemberVec_le5dec4&&>(self->records[self->count - 1]));
        self->records[self->count - 1].Free();
        base::global::CStrId::MoveConstructReloading(&self->ids[index], self->ids[self->count - 1]);
        self->ids[self->count - 1].~CStrId();
    }
    --self->count;
    return &self->records[index];
}
