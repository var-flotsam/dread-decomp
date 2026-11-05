// CMapAcquisitionComponent's unit: picks one of two members of the object at +0x7f8 by a flag (+0x539 or
// +0x7b9). Names are placeholders.
#include "types.h"

struct UnkMapAcquisitionTarget_b65a6 {
    u8 pad_0x0[0x100];
    u64 field_0x100;
    u64 field_0x108;
    u64 field_0x110;
    u8 pad_0x118[0x130 - 0x118];
    u64 field_0x130;
    u8 pad_0x138[0x1c8 - 0x138];
    u64 field_0x1c8;
    u64 field_0x1d0;
    u8 pad_0x1d8[0x1e0 - 0x1d8];
    u64 field_0x1e0;
    u64 field_0x1e8;
};

struct UnkMapAcquisitionOwner_b65a6 {
    u8 pad_0x0[0x539];
    bool flag_0x539;
    u8 pad_0x53a[0x7b9 - 0x53a];
    bool flag_0x7b9;
    u8 pad_0x7ba[0x7f8 - 0x7ba];
    UnkMapAcquisitionTarget_b65a6* target;
};

u64* fn_7100a85b98(UnkMapAcquisitionOwner_b65a6* self) { return self->flag_0x539 ? &self->target->field_0x1c8 : &self->target->field_0x100; }
u64* fn_7100a85bb4(UnkMapAcquisitionOwner_b65a6* self) { return self->flag_0x539 ? &self->target->field_0x1d0 : &self->target->field_0x110; }
u64* fn_7100a85bd0(UnkMapAcquisitionOwner_b65a6* self) { return self->flag_0x539 ? &self->target->field_0x1e8 : &self->target->field_0x108; }
u64* fn_7100a85bec(UnkMapAcquisitionOwner_b65a6* self) { return self->flag_0x7b9 ? &self->target->field_0x130 : &self->target->field_0x1e0; }
