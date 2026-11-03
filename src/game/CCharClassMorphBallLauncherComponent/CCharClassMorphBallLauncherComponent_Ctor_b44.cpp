// CCharClassMorphBallLauncherComponent (tools/tu_map.py; the name from reflection): the
// constructor. The base constructor fn_71011acd10, the vtable, cleared fields, a copy of the empty
// string id (g_unk_7101d470e0) at +0x70, then the vtable's slot at +0x18 on itself. Names and
// layout are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "types.h"

struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30* self);
extern void* g_unk_7101a7bd50[];  // the class's vtable

struct UnkMorphBallLauncherDef_b44a2 {
    void** vtable;
    u8 pad_0x8[0x50];
    u64 field_0x58;
    u64 field_0x60;
    u32 field_0x68;
    u32 pad_0x6c;
    base::global::CStrId field_0x70;
};

void fn_7100885304(UnkMorphBallLauncherDef_b44a2* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a7bd50 + 2;
    self->field_0x68 = 0;
    self->field_0x58 = 0;
    self->field_0x60 = 0;
    new (&self->field_0x70) base::global::CStrId(g_unk_7101d470e0);
    reinterpret_cast<void (*)(UnkMorphBallLauncherDef_b44a2*)>(self->vtable[3])(self);
}
