// Copy assignment of a record with two string ids around a counter (six identical copies).
#include "base/global/CStrId.h"
#include "game/UnkRec.h"
#include "types.h"

using base::global::CStrId;

UnkRec* fn_71002badc4(UnkRec* self, const UnkRec* other) {
    if (self != other) {
        self->field_0x8 = other->field_0x8;
        self->field_0x10 = other->field_0x10;
        self->field_0x18 = other->field_0x18;
    }
    return self;
}
