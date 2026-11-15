// Unit 2560 (tools/tu_map.py, no name hint): a destructor: four arrays (base::global::CRntVector's layout, at
// +0x10, +0x38, +0x60, +0x88) freed in reverse order, then the string id at +0x0 released in two steps.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkUnit2560Record {
    union {
        CStrId id;
    };
    u64 field_0x8;
    base::global::CRntVector<void*> field_0x10;
    base::global::CRntVector<void*> field_0x38;
    base::global::CRntVector<void*> field_0x60;
    base::global::CRntVector<void*> field_0x88;
};
static_assert(sizeof(UnkUnit2560Record) == 0xb0, "layout");

void fn_7100e1d538(UnkUnit2560Record* self) {
    self->field_0x88.Free();
    self->field_0x60.Free();
    self->field_0x38.Free();
    self->field_0x10.Free();
    self->id.Reset();
}
