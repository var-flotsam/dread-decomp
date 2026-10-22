// Move assignment of a record of two string ids (each released, then taking the other's; the source keeps
// the empty id).
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRecord {
    CStrId field_0x0;
    CStrId field_0x8;
};

UnkRecord* fn_71002db2a0(UnkRecord* self, UnkRecord* other) {
    if (self != other) {
        self->field_0x0.ReleaseAndTake(other->field_0x0);
        self->field_0x8.ReleaseAndTake(other->field_0x8);
    }
    return self;
}
