// CEnemyMovementWalkingAnims's unit (tools/tu_map.py, unit 1439): the copy-assignment of the walking
// animations' seven string ids, leaving the leading word alone. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkWalkingAnims_l968cc0 {
    void** vtable;
    CStrId field_0x8;
    CStrId field_0x10;
    CStrId field_0x18;
    CStrId field_0x20;
    CStrId field_0x28;
    CStrId field_0x30;
    CStrId field_0x38;
};

UnkWalkingAnims_l968cc0* fn_7100968cc0(UnkWalkingAnims_l968cc0* self, const UnkWalkingAnims_l968cc0* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x10 = other->field_0x10;
    self->field_0x18 = other->field_0x18;
    self->field_0x20 = other->field_0x20;
    self->field_0x28 = other->field_0x28;
    self->field_0x30 = other->field_0x30;
    self->field_0x38 = other->field_0x38;
    return self;
}
