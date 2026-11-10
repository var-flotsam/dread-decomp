// Unit 1175's destructor (tools/tu_map.py hint "CCentralUnitWeightedEdges"): an object with its own
// vtable, a string id at +0x8, a callback set (vtable 0x7101a20950, whose base class has vtable
// 0x7101a20998) at +0x10 and an id holder (fn_71002b24e8) at +0x58. Names and layouts are
// placeholders.
#include "base/UnkErasedVector.h"
#include "base/global/CStrId.h"
#include "game/UnkCallbackSetVector.h"
#include "types.h"

using base::global::CStrId;

extern void* g_unk_7101a6ec50[];
extern void* g_unk_7101a20950[];
extern void* g_unk_7101a20998[];
void fn_71002b24e8(void* node);

struct UnkCentralUnitEdges_ovb0259 {
    void** vtable;  // 0x0
    CStrId id;     // 0x8
    void** set_vtable;  // 0x10
    UnkOwnedErasedVector callbacks;  // 0x18
    u32 field_0x40;
    u8 pad_0x44[4];
    UnkRefCounted* owner;  // 0x48
    u8 pad_0x50[8];
    u8 node[0x10];  // 0x58
};

void fn_710076a66c(UnkCentralUnitEdges_ovb0259* self) {
    self->vtable = g_unk_7101a6ec50 + 2;
    fn_71002b24e8(self->node);
    self->set_vtable = g_unk_7101a20950 + 2;
    if (UnkRefCounted* p = self->owner) {
        if (--p->ref_count == 0)
            p->Delete();
    }
    self->set_vtable = g_unk_7101a20998 + 2;
    self->callbacks.~UnkOwnedErasedVector();
    self->id.Reset();
}
