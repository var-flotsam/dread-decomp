// animtree::CBoneControlDef unit (tools/tu_map.py, the name from reflection): the in-place constructor its
// reflection descriptor points to (fn_0x28), value-initialised (the whole object is cleared first): the base
// (vtable g_unk_7101a21718, the empty string id copied to +0x8, as
// src/game/animtree/CSamusLegsBoneControlDef/CSamusLegsBoneControlDef_Ctor_b67a2.cpp), then this class's vtable
// and a second, default string id at +0x10 (the pool is created first if there is none). Defined under the
// `void()` symbol src/reflection/animtree_CBoneControlDef.cpp declares. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/animtree/CAnimatedNodeDef.h"

using base::global::CStrId;
using base::global::UnkStringData;

extern void* g_unk_7101a21af8[];

struct UnkBoneControlDef_A2_b76 {
    char* vtable;
    CStrId name;          // 0x8
    UnkStringData* bone;  // 0x10
    u64 field_0x18;
    u64 field_0x20;
};

void fn_71002e1ad4_A2_b76(UnkBoneControlDef_A2_b76* self) asm("_Z13fn_71002e1ad4v");
void fn_71002e1ad4_A2_b76(UnkBoneControlDef_A2_b76* self) {
    __builtin_memset(self, 0, sizeof(*self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21718) + 0x10;
    new (&self->name) CStrId(g_unk_7101d470e0);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21af8) + 0x10;
    self->bone = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->bone = &g_unk_7101d47108->empty;
}
