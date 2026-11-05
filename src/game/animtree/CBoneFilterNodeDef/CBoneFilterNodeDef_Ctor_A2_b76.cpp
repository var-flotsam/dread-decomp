// animtree::CBoneFilterNodeDef unit (tools/tu_map.py, the name from reflection): the in-place constructor its
// reflection descriptor points to (fn_0x28), value-initialised (the whole object is cleared first): the base
// (vtable g_unk_7101a21718, the empty string id copied to +0x8, as
// src/game/animtree/CSamusLegsBoneControlDef/CSamusLegsBoneControlDef_Ctor_b67a2.cpp), then this class's vtable
// and two arrays (flags 0xf) at +0x10 and +0x38. Defined under the `void()` symbol
// src/reflection/animtree_CBoneFilterNodeDef.cpp declares. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::global::CStrId;

extern void* g_unk_7101a21718[];
extern void* g_unk_7101a21c70[];

struct UnkBoneFilterNodeDef_A2_b76 {
    char* vtable;
    CStrId name;                      // 0x8
    base::UnkVector<u64> field_0x10;  // 0x10
    base::UnkVector<u64> field_0x38;  // 0x38
};

void fn_71002e319c_A2_b76(UnkBoneFilterNodeDef_A2_b76* self) asm("_Z13fn_71002e319cv");
void fn_71002e319c_A2_b76(UnkBoneFilterNodeDef_A2_b76* self) {
    __builtin_memset(self, 0, sizeof(*self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21718) + 0x10;
    new (&self->name) CStrId(g_unk_7101d470e0);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21c70) + 0x10;
    new (&self->field_0x10) base::UnkVector<u64>(0xf);
    new (&self->field_0x38) base::UnkVector<u64>(0xf);
}
