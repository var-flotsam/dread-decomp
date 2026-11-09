// behaviortree::CComposite's unit (tools/tu_map.py): the composite's destructor: its vtable, each child
// in the array at +0x30 destroyed through its vf_0x10, the array emptied and freed, then the base
// node's destructor (fn_7100394ac8). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a27068[];

struct UnkBtNode_ovb0069 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
};

struct UnkBtComposite_ovb0069 {
    void** vtable;
    u8 pad_0x8[0x28];
    base::UnkVector<UnkBtNode_ovb0069*> children;  // 0x30
};

void fn_7100394ac8(UnkBtComposite_ovb0069* self);

void fn_71003961e8(UnkBtComposite_ovb0069* self) {
    self->vtable = g_unk_7101a27068 + 2;
    for (UnkBtNode_ovb0069** it = self->children.data; it != self->children.data + self->children.count; ++it)
        if (*it)
            (*it)->vf_0x10();
    self->children.count = 0;
    self->children.Free();
    fn_7100394ac8(self);
}
