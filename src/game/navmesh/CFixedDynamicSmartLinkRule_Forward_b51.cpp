// navmesh::CFixedDynamicSmartLinkRule unit (tools/tu_map.py, the name from reflection): forwards to
// the rule's virtual at 0x38, unpacking the link (+0x60 and +0x190) and the cell (+0x100, +0x102)
// in between the pass-through arguments. Names and layouts are placeholders.
#include "types.h"

struct UnkSmartLinkSource {
    u8 pad_0x0[0x60];
    u32 id;  // 0x60
    u8 pad_0x64[0x12c];
    void* data;  // 0x190
};

struct UnkSmartLinkCell {
    u8 pad_0x0[0x100];
    u16 index;  // 0x100
    u8 kind;  // 0x102
};

class UnkSmartLinkRule {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38(void* a, void* b, u32 id, u16 index, u8 kind, void* data, void* c, u32 d,
                         u32 e);
};

void fn_710026f644(UnkSmartLinkRule* self, void* a, void* b, const UnkSmartLinkSource* source,
                   const UnkSmartLinkCell* cell, void* c, u32 d, u32 e) {
    self->vf_0x38(a, b, source->id, cell->index, cell->kind, source->data, c, d, e);
}
