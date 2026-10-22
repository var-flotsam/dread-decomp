// Move assignment of a record made of an array (UnkVector, whose elements are destroyed first) and a
// string id (released like a destructor would, then taking the other's; the other is left with the
// empty id).
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord {
    base::UnkVector<void*> field_0x0;
    CStrId field_0x28;
};

UnkRecord* fn_71003eeca8(UnkRecord* self, UnkRecord* other) {
    if (self != other) {
        self->field_0x0 = static_cast<base::UnkVector<void*>&&>(other->field_0x0);
        self->field_0x28.ReleaseAndTake(other->field_0x28);
    }
    return self;
}
