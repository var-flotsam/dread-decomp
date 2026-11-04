// The copy constructors of UnkUnit2601_VecCopy_b56.h.
#include "UnkUnit2601_VecCopy_b56.h"

void fn_7100f85f00(UnkPathRecordVec_b56* self, const UnkPathRecordVec_b56* other) { self->CopyFrom(*other); }

void fn_7100f85c28(UnkRecord50Vec_b56* self, const UnkRecord50Vec_b56* other) { self->CopyFrom(*other); }

UnkRecord50Vec_b56* fn_7100f86254(UnkRecord50Vec_b56* self, const UnkRecord50Vec_b56* other) {
    if (self != other) {
        fn_7100d90460(self, 0);
        self->AppendAll(*other);
    }
    return self;
}
