// CCharClassActionSwitcherComponent (tools/tu_map.py; the name from reflection): the constructor.
// The object is cleared first, then the base constructor fn_71011acd10 runs; the vtable, two copies
// of the empty string id (g_unk_7101d470e0, the id and the pool's empty one read once, as in
// src/game/CBarelyFrozenIceInfo/CBarelyFrozenIceInfo_Ctor_b44.cpp) at +0x58 and +0x60, a cleared
// half-word at +0x68, then the vtable's slot at +0x18 on itself. Names and layout are placeholders.
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkStruct_71008bca30;
extern void* g_unk_7101a7aa50[];  // the class's vtable

struct UnkActionSwitcherDef_b44a2 {
    void** vtable;
    u8 pad_0x8[0x50];
    UnkStringData* field_0x58;  // CStrId
    UnkStringData* field_0x60;  // CStrId
    u16 field_0x68;
};

void fn_7100877574(UnkActionSwitcherDef_b44a2* self) {
    __builtin_memset(self, 0, 0x58);
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a7aa50 + 2;
    UnkStringData* id = g_unk_7101d470e0.data;
    self->field_0x58 = id;
    bool counted = &g_unk_7101d47108->empty != id;
    if (counted)
        ++id->ref_count;
    self->field_0x60 = id;
    if (counted)
        ++id->ref_count;
    self->field_0x68 = 0;
    reinterpret_cast<void (*)(UnkActionSwitcherDef_b44a2*)>(self->vtable[3])(self);
}
