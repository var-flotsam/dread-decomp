// animtree::CAnimatedNodeDef unit (tools/tu_map.py, the name from reflection): the in-place constructor its
// reflection descriptor points to (fn_0x28), value-initialised (the whole object is cleared first): the base
// (vtable g_unk_7101a21718, the empty string id copied to +0x8, as
// src/game/animtree/CSamusLegsBoneControlDef/CSamusLegsBoneControlDef_Ctor_b67a2.cpp), then this class's vtable
// and a word at +0x10 and two arrays (flags 0xf) at +0x18 and +0x40 (0x70 bytes). Defined under the `void()` symbol
// src/reflection/animtree_CAnimatedNodeDef.cpp declares. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::global::CStrId;

extern void* g_unk_7101a21718[];
extern void* g_unk_7101a216e8[];

struct UnkAnimatedNodeDef_A2_b76 {
    char* vtable;
    CStrId name;                      // 0x8
    u64 field_0x10;
    base::UnkVector<u64> field_0x18;  // 0x18
    base::UnkVector<u64> field_0x40;  // 0x40
    u64 field_0x68;
};

void fn_71002d403c_A2_b76(UnkAnimatedNodeDef_A2_b76* self) asm("_Z13fn_71002d403cv");
void fn_71002d403c_A2_b76(UnkAnimatedNodeDef_A2_b76* self) {
    __builtin_memset(self, 0, sizeof(*self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21718) + 0x10;
    new (&self->name) CStrId(g_unk_7101d470e0);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a216e8) + 0x10;
    new (&self->field_0x18) base::UnkVector<u64>(0xf);
    new (&self->field_0x40) base::UnkVector<u64>(0xf);
}
