// Destructor of a component: sets its vtable, destroys a member at +0x2a710 (fn_71004f11f8), frees two
// arrays (+0x2a718, +0x2a710) through the allocator at +0x2a730, releases the string id at +0x2a708 and
// finishes with the base destructor (fn_710053f510).
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a43038[];

struct UnkMember {
    void* field_0x0;
};

struct UnkTailBlock {
    CStrId field_0x0;
    UnkMember field_0x8;
    void* field_0x10;
    u8 pad_0x18[0x10];
    base::UnkAllocator* field_0x28;
};

struct UnkComponent {
    void** vtable;
    u8 pad_0x8[0x2a700];
    UnkTailBlock field_0x2a708;
};

void fn_71004f11f8(UnkMember* member);
void fn_710053f510(UnkComponent* self);

void fn_71004f0428(UnkComponent* self) {
    self->vtable = g_unk_7101a43038 + 2;
    fn_71004f11f8(&self->field_0x2a708.field_0x8);
    if (self->field_0x2a708.field_0x10)
        self->field_0x2a708.field_0x28->vf_0x48(self->field_0x2a708.field_0x10);
    if (self->field_0x2a708.field_0x8.field_0x0)
        self->field_0x2a708.field_0x28->vf_0x48(self->field_0x2a708.field_0x8.field_0x0);
    self->field_0x2a708.field_0x0.Reset();
    fn_710053f510(self);
}
