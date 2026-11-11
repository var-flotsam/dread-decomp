// CCharClassPickableComponent's unit (tools/tu_map.py unit 1491): the constructor (fn_71009cc968): the
// base constructor fn_71011acd10, the vtable, an empty path at +0x58 and an empty id at +0x60, four
// "Energy Fragment acquired." ids (+0x68 to +0x80), "Tank Unknown" at +0x88, then the vtable's slot
// at +0x18 on itself. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkStruct_71008bca30;
extern void* g_unk_7101a89160[];  // the class's vtable

struct UnkPickableDef_ovb0090 {
    void** vtable;
    u8 pad_0x8[0x50];
    CFilePathStrId field_0x58;
    CStrId field_0x60;
    CStrId field_0x68;
    CStrId field_0x70;
    CStrId field_0x78;
    CStrId field_0x80;
    CStrId field_0x88;

    void vf_0x18() { reinterpret_cast<void (*)(UnkPickableDef_ovb0090*)>(vtable[3])(this); }
};

void fn_71009cc968(UnkPickableDef_ovb0090* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a89160 + 2;
    new (&self->field_0x58) CFilePathStrId(g_unk_710162e2ed, true);
    new (&self->field_0x60) CStrId(g_unk_710162e2ed, true);
    new (&self->field_0x68) CStrId("Energy Fragment acquired.", true);
    new (&self->field_0x70) CStrId("Energy Fragment acquired.", true);
    new (&self->field_0x78) CStrId("Energy Fragment acquired.", true);
    new (&self->field_0x80) CStrId("Energy Fragment acquired.", true);
    new (&self->field_0x88) CStrId("Tank Unknown", true);
    self->vf_0x18();
}
