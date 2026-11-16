// CMinimapData's unit (2563): move assignment of a holder of a string id and a vector of string ids,
// held either in place (kind 1) or through pointers to them (kind 2). Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::UnkStrIdMember;
using base::global::CStrId;
using UnkStrIdMemberVec_le5c990 = base::global::CRntVector<UnkStrIdMember>;

// The id and the vector in place.
struct UnkIdVecValue_le5c990 {
    CStrId id;
    UnkStrIdMemberVec_le5c990 values;
};

// Pointers to an id and a vector held elsewhere.
struct UnkIdVecRef_le5c990 {
    CStrId* id;
    UnkStrIdMemberVec_le5c990* values;
};

struct UnkIdVecHolder_le5c990 {
    alignas(8) u8 storage[0x30];  // an UnkIdVecValue_le5c990 (kind 1) or an UnkIdVecRef_le5c990 (kind 2)
    u32 kind;

    UnkIdVecValue_le5c990& Value() { return *reinterpret_cast<UnkIdVecValue_le5c990*>(storage); }
    UnkIdVecRef_le5c990& Ref() { return *reinterpret_cast<UnkIdVecRef_le5c990*>(storage); }
    CStrId& Id() { return kind == 2 ? *Ref().id : Value().id; }
    UnkStrIdMemberVec_le5c990& Values() { return kind == 2 ? *Ref().values : Value().values; }
};
static_assert(sizeof(UnkIdVecHolder_le5c990) == 0x38, "");

void* fn_7100e5c990(void* dst, void* src) {
    UnkIdVecHolder_le5c990* self = static_cast<UnkIdVecHolder_le5c990*>(dst);
    UnkIdVecHolder_le5c990* other = static_cast<UnkIdVecHolder_le5c990*>(src);
    switch (other->kind) {
    case 1:
        self->Id().ReleaseAndTake(other->Value().id);
        self->Values() = static_cast<UnkStrIdMemberVec_le5c990&&>(other->Value().values);
        break;
    case 2:
        self->Id().ReleaseAndTake(*other->Ref().id);
        self->Values() = static_cast<UnkStrIdMemberVec_le5c990&&>(*other->Ref().values);
        break;
    }
    return self;
}
