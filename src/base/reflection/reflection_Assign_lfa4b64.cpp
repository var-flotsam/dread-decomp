// base::reflection's unit (tools/tu_map.py, unit 2610): the copy-assignment of a record of nine string
// ids (fn_7100fa4b64), which does nothing when assigned to itself. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkStrIdRecord_lfa4b64 {
    CStrId field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    CStrId field_0x20;
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
    CStrId field_0x40;
};

UnkStrIdRecord_lfa4b64* fn_7100fa4b64(UnkStrIdRecord_lfa4b64* self, const UnkStrIdRecord_lfa4b64* other) {
    if (self != other) {
        self->field_0x0 = other->field_0x0;
        self->field_0x8 = other->field_0x8;
        self->field_0x10 = other->field_0x10;
        self->field_0x18 = other->field_0x18;
        self->field_0x20 = other->field_0x20;
        self->field_0x28 = other->field_0x28;
        self->field_0x30 = other->field_0x30;
        self->field_0x38 = other->field_0x38;
        self->field_0x40 = other->field_0x40;
    }
    return self;
}
