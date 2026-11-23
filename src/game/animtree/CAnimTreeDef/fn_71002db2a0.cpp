// Move assignment of a record of two string ids (each released, then taking the other's; the source keeps
// the empty id).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord_2db2a0 {
    CStrId field_0x0;
    CStrId field_0x8;
};

UnkRecord_2db2a0* fn_71002db2a0(UnkRecord_2db2a0* self, UnkRecord_2db2a0* other) {
    if (self != other) {
        self->field_0x0.ReleaseAndTake(other->field_0x0);
        self->field_0x8.ReleaseAndTake(other->field_0x8);
    }
    return self;
}
