// CCharClassFXLinkEvent's unit (tools/tu_map.py unit 1790): the event's copy assignment (fn_7100bb2e24): every
// member but the vtable, the three string ids (+0x10, +0x20, +0x40) assigned as ids. Names and layouts are
// placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkFXLinkTriple_ovb0097 {
    u64 a, b, c;
};

struct UnkFXLinkEntry_ovb0097 {
    void* vtable;
    u32 field_0x8;
    CStrId field_0x10;
    u32 field_0x18;
    CStrId field_0x20;
    UnkFXLinkTriple_ovb0097 field_0x28;
    CStrId field_0x40;
};

UnkFXLinkEntry_ovb0097* fn_7100bb2e24(UnkFXLinkEntry_ovb0097* self, const UnkFXLinkEntry_ovb0097* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x10 = other->field_0x10;
    self->field_0x18 = other->field_0x18;
    self->field_0x20 = other->field_0x20;
    self->field_0x28 = other->field_0x28;
    self->field_0x40 = other->field_0x40;
    return self;
}
