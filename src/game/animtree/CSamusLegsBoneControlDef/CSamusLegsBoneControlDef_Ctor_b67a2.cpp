// animtree::CSamusLegsBoneControlDef unit (tools/tu_map.py): the default constructor of the bone control
// definition, value-initialised (the whole object is cleared first, as `new (p) T()` does): the base (vtable
// g_unk_7101a21718, the empty string id g_unk_7101d470e0 copied to +0x8), then this class's vtable
// (g_unk_7101a221d0) and -1 in the words at +0x10..+0x28.
// The copy constructors are in CSamusLegsBoneControlDef_CopyCtor_b48.cpp. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/animtree/CAnimatedNodeDef.h"
#include "game/animtree/CSamusLegsBoneControlDef.h"

using base::global::CStrId;

struct UnkSamusLegsBoneControlDef_b67a2 {
    char* vtable;
    CStrId name;  // 0x8
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
};

void fn_71002e9308(UnkSamusLegsBoneControlDef_b67a2* self) {
    __builtin_memset(self, 0, sizeof(*self));
    self->vtable = reinterpret_cast<char*>(g_unk_7101a21718) + 0x10;
    new (&self->name) CStrId(g_unk_7101d470e0);
    self->vtable = g_unk_7101a221d0 + 0x10;
    __builtin_memset(&self->field_0x10, 0xff, 0x18);
}
