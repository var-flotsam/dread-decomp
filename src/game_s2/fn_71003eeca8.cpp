// Move assignment of a record made of an array (CRntVector, whose elements are destroyed first) and a
// string id (released like a destructor would, then taking the other's; the other is left with the
// empty id).
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord_3eeca8 {
    base::global::CRntVector<void*> field_0x0;
    CStrId field_0x28;
};

UnkRecord_3eeca8* fn_71003eeca8(UnkRecord_3eeca8* self, UnkRecord_3eeca8* other) {
    if (self != other) {
        self->field_0x0 = static_cast<base::global::CRntVector<void*>&&>(other->field_0x0);
        self->field_0x28.ReleaseAndTake(other->field_0x28);
    }
    return self;
}
