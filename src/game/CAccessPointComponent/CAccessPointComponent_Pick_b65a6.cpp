// CAccessPointComponent's unit: picks one of two members of the object at +0x698 by the flag at +0x590.
// Names are placeholders.
#include "types.h"

struct UnkAccessPointTarget_b65a6 {
    u8 pad_0x0[0x100];
    u64 field_0x100;
    u64 field_0x108;
    u64 field_0x110;
    u8 pad_0x118[0x1c8 - 0x118];
    u64 field_0x1c8;
    u64 field_0x1d0;
    u64 field_0x1d8;
};

struct UnkAccessPointOwner_b65a6 {
    u8 pad_0x0[0x590];
    bool flag;
    u8 pad_0x591[0x698 - 0x591];
    UnkAccessPointTarget_b65a6* target;
};

u64* fn_7100a6e838(UnkAccessPointOwner_b65a6* self) { return self->flag ? &self->target->field_0x1c8 : &self->target->field_0x100; }
u64* fn_7100a6e854(UnkAccessPointOwner_b65a6* self) { return self->flag ? &self->target->field_0x1d0 : &self->target->field_0x110; }
u64* fn_7100a6e870(UnkAccessPointOwner_b65a6* self) { return self->flag ? &self->target->field_0x1d8 : &self->target->field_0x108; }
