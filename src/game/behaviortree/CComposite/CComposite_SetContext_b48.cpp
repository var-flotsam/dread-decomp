// behaviortree::CComposite unit (tools/tu_map.py): stores the context at +0x10 and passes it on to
// every child (virtual slot 0x48), the children array (+0x30, count +0x38) re-read after each call.
// fn_710039672c does the same for the field at +0x18 and slot 0x50. Names and layouts are placeholders.
#include "types.h"

struct UnkBtContext;

struct UnkBtNode {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(UnkBtContext* context);
    virtual void vf_0x50(UnkBtContext* context);
};

struct UnkBtComposite {
    u8 pad_0x0[0x10];
    UnkBtContext* context;  // 0x10
    UnkBtContext* field_0x18;
    u8 pad_0x20[0x10];
    UnkBtNode** children;   // 0x30
    u32 count;              // 0x38
};

void fn_71003966cc(UnkBtComposite* self, UnkBtContext* context) {
    self->context = context;
    for (UnkBtNode** it = self->children; it != self->children + self->count; ++it)
        (*it)->vf_0x48(context);
}

void fn_710039672c(UnkBtComposite* self, UnkBtContext* value) {
    self->field_0x18 = value;
    for (UnkBtNode** it = self->children; it != self->children + self->count; ++it)
        (*it)->vf_0x50(value);
}
